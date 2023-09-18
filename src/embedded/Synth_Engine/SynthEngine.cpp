#include "Arduino.h"
#include "SynthEngine.h"
#include "esp_task_wdt.h"


/*
 * SynthEngine Library by Federico Longhin
 * see https://github.com/FedericoLonghin/Electronic-Organ
 */

/*
 * Auxiliary variables list:
 */

// AudioCompositorHandler()
int uselessCounter = 0;

// generateAudioChunk()
byte sampleVal;
byte ampl;
float trem;
float oldtrem;
unsigned int totalWaveVal;
unsigned int totalWaveVal_ch;
float divider = Wavetable_Length / ((float)(Sample_Rate));


// reloadWavetable()
float drowBarsMolt[9] = { 1, 3, 2, 4, 6, 8, 10, 12, 16 };
float val;

// IntSr()
byte wave[MAGIC_BUFFER_OFFSET * 2];
bool newSampleREQ;
bool newSampleREQ_SectionToFill;
unsigned int OutBufferIndex = 0;

// setupTimerInterrupt
hw_timer_t *Timer0_Cfg = NULL;

/*
 * Methods and functions:
 */

SynthEngine::SynthEngine() {
  newSampleREQ = false;
  newSampleREQ_SectionToFill = false;
  for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
    _currentlyPlayingNote_perChannel[ch] = 0;
  }
  calc_currentlyPlayingNote_total();
}

bool SynthEngine::start(int note, int channel) {
  if (note < 0) return 0;
  int locat = find_inActiveNoteList(note, channel);
  if (locat != -1) {  //this note is already present
    // Serial.println("New note is already present");
    Envelope *env = &soundList[channel].ADSR;
    byte ampl = env->getAmplitude(AudioObjectList[getNoteId(note, channel)]->ticksFromLastEvent, 0, AudioObjectList[getNoteId(note, channel)]->releaseStartingPoint, &AudioObjectList[getNoteId(note, channel)]->toBeDeleted);
    int envelopeIndexStart = env->getAttackIndex(ampl);
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note + ObjectChannelShift[channel], envelopeIndexStart * (Sample_Rate / 1000), channel);
    // Serial.printf("Ampl:%d\tStart:%d\n", ampl, envelopeIndexStart);
  } else {
    if (_currentlyPlayingNote_total >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note + ObjectChannelShift[channel], 0L, channel);
    NewactiveNoteList[channel][_currentlyPlayingNote_perChannel[channel]] = getNoteId(note, channel);
    _currentlyPlayingNote_perChannel[channel]++;
    calc_currentlyPlayingNote_total();
  }
  return true;
}

bool SynthEngine::release(int note, int channel) {
  int id = getNoteId(note, channel);
  Envelope *env = &soundList[channel].ADSR;

  AudioObjectList[id]->releaseStartingPoint = env->getReleaseIndex(env->getAmplitude(AudioObjectList[id]->ticksFromLastEvent, true, 0, 0));
  AudioObjectList[id]->isKeyPressed = false;
  AudioObjectList[id]->ticksFromLastEvent = 0;

  return true;
}

bool SynthEngine::stop(int pos_in_activeNoteList, int channel) {

  if (pos_in_activeNoteList == -1) return false;
  if (_currentlyPlayingNote_perChannel[channel] > 1 && pos_in_activeNoteList != _currentlyPlayingNote_perChannel[channel] - 1) {
    activeNoteList_Change_REQ = true;
    while (!activeNoteList_Change_ACK) {
      // uselessCounter++;
      esp_task_wdt_reset();
    }
    NewactiveNoteList[channel][pos_in_activeNoteList] = NewactiveNoteList[channel][_currentlyPlayingNote_perChannel[channel] - 1];
    activeNoteList_Change_REQ = false;
  }
  _currentlyPlayingNote_perChannel[channel]--;
  calc_currentlyPlayingNote_total();
  return true;
}

void SynthEngine::cleanSilentObjects() {
  for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
    int num = _currentlyPlayingNote_perChannel[ch];
    for (int i = 0; i < num; i++) {
      if (AudioObjectList[NewactiveNoteList[ch][i]]->toBeDeleted) {
        // Serial.printf("Stopping %d\n", i);
        stop(i, ch);
      }
    }
  }
}

int SynthEngine::find_inActiveNoteList(int note, int channel) {
  int id = getNoteId(note, channel);
  for (int i = 0; i < _currentlyPlayingNote_perChannel[channel]; i++) {
    if (NewactiveNoteList[channel][i] == id) return i;
  }
  return -1;
}

void SynthEngine::stopAll() {
  for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
    _currentlyPlayingNote_perChannel[ch] = 0;
  }
  calc_currentlyPlayingNote_total();
}

int SynthEngine::getNoteId(int note, int channel) {
  return channel * MAX_AUDIO_OBJECT_PER_CHANNEL + note;
}

void SynthEngine::calc_currentlyPlayingNote_total() {
  int val = 0;
  for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
    val += _currentlyPlayingNote_perChannel[ch];
  }
  _currentlyPlayingNote_total = val;
}

void SynthEngine::reloadWavetable() {


  // Double Frequency Sin
  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SIN][i] = sin((i / (float)Wavetable_Length) * 4 * PI) * Wavetable_MaxAmplitude_val / 4.0f + Wavetable_MaxAmplitude_val / 2;
  }

  // Double Frequency Triang
  for (int i = 0; i < Wavetable_Length; i++) {
    if (i <= Wavetable_Length / 2) {
      if (i <= Wavetable_Length / 4) {
        Wavetable_table[WAVETYPE_TRIANG][i] = ((i / (float)(Wavetable_Length / 4)) * (Wavetable_MaxAmplitude_val / 2)) + Wavetable_MaxAmplitude_val / 4;

      } else {
        Wavetable_table[WAVETYPE_TRIANG][i] = Wavetable_table[WAVETYPE_TRIANG][Wavetable_Length / 2 - i];
      }
    } else
      Wavetable_table[WAVETYPE_TRIANG][i] = Wavetable_table[WAVETYPE_TRIANG][i - Wavetable_Length / 2];
  }

  // Double Frequency Saw
  for (int i = 0; i < Wavetable_Length; i++) {
    if (i <= Wavetable_Length / 2) {
      Wavetable_table[WAVETYPE_SAW][i] = (i / (float)(Wavetable_Length / 2)) * Wavetable_MaxAmplitude_val / 2.0f;
    } else {
      Wavetable_table[WAVETYPE_SAW][i] = Wavetable_table[WAVETYPE_SAW][i - Wavetable_Length / 2];
    }
  }

  // Double Frequency Square
  for (int i = 0; i < Wavetable_Length; i++) {
    if (i <= Wavetable_Length / 2) {
      Wavetable_table[WAVETYPE_SQUARE][i] = i < Wavetable_Length / 4 ? Wavetable_MaxAmplitude_val * 3 / 4.0f : Wavetable_MaxAmplitude_val * 1 / 4.0f;
    } else {
      Wavetable_table[WAVETYPE_SQUARE][i] = Wavetable_table[WAVETYPE_SQUARE][i - Wavetable_Length / 2];
    }
  }

  // Double Frequency Organ
  for (int i = 0; i < Wavetable_Length; i++) {
    val = 0;
    for (int b = 0; b < 9; b++) {
      if (drowBarsStat[b])
        val += sin((i / (float)Wavetable_Length) * drowBarsMolt[b] * 2 * PI);
    }
    Wavetable_table[WAVETYPE_ORGAN][i] = val * Wavetable_MaxAmplitude_val / 12.0f + Wavetable_MaxAmplitude_val / 2;
  }
}

//just for debug
void startCounter(int max, bool count_first);
void endCounter();
void tick();

void SynthEngine::AudioCompositorHandler() {
#if 0  //testbench
  while (1) {
    while (!getActiveNotesNumber()) {
      esp_task_wdt_reset();
    }
#define counterLen 200000
    int testVal = 0;
    soundList[0].Trem.begin(1,1);
    soundList[0].Trem.enable;
    startCounter(counterLen, false);
    for (int i = 0; i < counterLen; i++) {
      int ObjAddr = activeNoteList[0];
      trem = soundList[0].Trem.getValNew(true);//...ms

      testVal += trem;
      // tick();
    }
    endCounter();
    Serial.println(testVal);
  }
#endif
  for (;;) {
    if (newSampleREQ) {
      newSampleREQ = false;
      generateAudioChunk(MAGIC_BUFFER_OFFSET, newSampleREQ_SectionToFill);
    }
    if (activeNoteList_Change_REQ) {
      activeNoteList_Change_ACK = true;
      while (activeNoteList_Change_REQ) {
        // uselessCounter++;
        // if (uselessCounter > 100) uselessCounter = 0;
        esp_task_wdt_reset();
      }
      activeNoteList_Change_ACK = false;
    }
    uselessCounter++;  //just for triggering WDT
  }
}



void SynthEngine::generateAudioChunk(int len, bool _section) {
  int noteNum[AUDIO_OBJECT_CHANNEL];
  for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
    noteNum[ch] = _currentlyPlayingNote_perChannel[ch];
  }

  for (int a = 0; a < len; a++) {

    totalWaveVal = 0;
    for (int ch = 0; ch < AUDIO_OBJECT_CHANNEL; ch++) {
      totalWaveVal_ch = 0;
      for (int f = 0; f < noteNum[ch]; f++) {
        // FillBufferIndex = 0;
        int ObjAddr = NewactiveNoteList[ch][f];
        int _soundNum = AudioObjectList[ObjAddr]->sound;

        sampleVal = Wavetable_table[soundList[_soundNum].Wavetype][(int)(((int)((AudioObjectList[ObjAddr]->frequency) / 2 * FillBufferIndex ) % (Sample_Rate)) * divider)];
        ampl = soundList[_soundNum].ADSR.getAmplitudeNew(AudioObjectList[ObjAddr]);

        totalWaveVal_ch += (sampleVal * ampl);
      }
      trem = soundList[ch].Trem.getVal(true);
      totalWaveVal += totalWaveVal_ch * trem;
    }
    totalWaveVal /= 1200;
    wave[MAGIC_BUFFER_OFFSET * _section + a] = totalWaveVal < 255 ? totalWaveVal : 255;
    FillBufferIndex++;
    if (FillBufferIndex >= Sample_Rate) {
      FillBufferIndex = 0;
    }
  }
}

void IRAM_ATTR IntSR() {
  if (OutBufferIndex >= MAGIC_BUFFER_OFFSET * 2) {
    OutBufferIndex = 0;

    newSampleREQ = true;
    newSampleREQ_SectionToFill = true;
  } else if (OutBufferIndex == MAGIC_BUFFER_OFFSET) {
    newSampleREQ = true;
    newSampleREQ_SectionToFill = false;
  }

  dacWrite(25, wave[OutBufferIndex]);
  OutBufferIndex++;
}

void setupTimerInterrupt() {
  Timer0_Cfg = timerBegin(0, 2, true);
  timerAttachInterrupt(Timer0_Cfg, *IntSR, true);
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);
}


float lowpass_filter(float input, float alpha, float prevInput, float prevOutput) {
  float output = alpha * input + (1.0 - alpha) * prevOutput;
  return output;
}

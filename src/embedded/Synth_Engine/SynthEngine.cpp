#include "Arduino.h"
#include "SynthEngine.h"


/*
 * SynthEngine Library by Federico Longhin
 * see https://github.com/FedericoLonghin/Electronic-Organ
 */

/*
 * Auxiliary variables list:
 */

// AudioCompositorHandler()
int c = 0;

// generateAudioChunk()
byte sampleVal;
byte ampl;
unsigned int totalWaveVal;
float divider = Wavetable_Length / (float)(Sample_Rate);

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
  _currentlyPlayingNote = 0;
  newSampleREQ = false;
  newSampleREQ_SectionToFill = false;
}


bool SynthEngine::start(int note, int channel) {
  if (note < 0) return 0;
  int locat = find_inActiveNoteList(getNoteId(note, channel));
  if (locat != -1) {  //this note is already present
    Serial.println("New note is already present");
    Envelope *env = &soundList[channel].ADSR;
    byte ampl = env->getAmplitude(AudioObjectList[getNoteId(note, channel)]->ticksFromLastEvent, 0, AudioObjectList[getNoteId(note, channel)]->releaseStartingPoint, &AudioObjectList[getNoteId(note, channel)]->toBeDeleted);
    int envelopeIndexStart = env->getAttackIndex(ampl);
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note + ObjectChannelShift[channel], envelopeIndexStart * (Sample_Rate / 1000), channel);
    Serial.printf("Ampl:%d\tStart:%d\n", ampl, envelopeIndexStart);
  } else {
    if (_currentlyPlayingNote >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note + ObjectChannelShift[channel], 0L, channel);
    activeNoteList[_currentlyPlayingNote] = getNoteId(note, channel);
    _currentlyPlayingNote++;
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

bool SynthEngine::stop(int id) {
  Serial.printf("Deleting note:%d\n", id);
  int locat = find_inActiveNoteList(id);
  if (locat == -1) return false;
  if (_currentlyPlayingNote > 1) {
    activeNoteList[locat] = activeNoteList[_currentlyPlayingNote - 1];
  }
  _currentlyPlayingNote--;
  return true;
}

void SynthEngine::cleanSilentObjects() {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    if (AudioObjectList[activeNoteList[i]]->toBeDeleted) {
      stop(activeNoteList[i]);
    }
  }
}

int SynthEngine::find_inActiveNoteList(int id) {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    if (activeNoteList[i] == id) return i;
  }
  return -1;
}

int SynthEngine::getActiveNotesNumber() {
  return _currentlyPlayingNote;
}

void SynthEngine::stopAll() {
  _currentlyPlayingNote = 0;
}

int SynthEngine::getNoteId(int note, int channel) {
  return channel * MAX_AUDIO_OBJECT_PER_CHANNEL + note;
}

void SynthEngine::reloadWavetabl#definee() {

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SIN][i] = sin((i / (float)Wavetable_Length) * 2 * PI) * Wavetable_MaxAmplitude_val / 4.0f + Wavetable_MaxAmplitude_val / 2;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    if (i <= Wavetable_Length / 2) {
      Wavetable_table[WAVETYPE_TRIANG][i] = ((i / (float)(Wavetable_Length / 2)) * (Wavetable_MaxAmplitude_val / 2)) + Wavetable_MaxAmplitude_val / 4;

    } else {
      Wavetable_table[WAVETYPE_TRIANG][i] = Wavetable_table[WAVETYPE_TRIANG][Wavetable_Length - i];
    }
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SAW][i] = (i / (float)Wavetable_Length) * Wavetable_MaxAmplitude_val / 4.0f;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SQUARE][i] = i < Wavetable_Length / 2 ? Wavetable_MaxAmplitude_val * 3 / 4.0f : Wavetable_MaxAmplitude_val * 1 / 4.0f;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SIN_3][i] = (sin((i / (float)Wavetable_Length) * 2 * PI) + sin((i / (float)Wavetable_Length) * 4 * PI) + sin((i / (float)Wavetable_Length) * 8 * PI)) * Wavetable_MaxAmplitude_val / 12.0f + Wavetable_MaxAmplitude_val / 2;
  }
}

void SynthEngine::AudioCompositorHandler() {
  for (;;) {
    if (newSampleREQ) {
      newSampleREQ = false;
      generateAudioChunk(MAGIC_BUFFER_OFFSET, newSampleREQ_SectionToFill);
    }
    c++;  //just for triggering WDT
  }
}

void SynthEngine::generateAudioChunk(int len, bool _section) {
  int noteNum = getActiveNotesNumber();
  for (int a = 0; a < len; a++) {

    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      int ObjAddr = activeNoteList[f];
      sampleVal = Wavetable_table[soundList[AudioObjectList[ObjAddr]->sound].Wavetype][(byte)((((AudioObjectList[ObjAddr]->frequency) * FillBufferIndex) % (Sample_Rate)) * divider)];
      ampl = soundList[AudioObjectList[ObjAddr]->sound].ADSR.getAmplitude(AudioObjectList[ObjAddr]->ticksFromLastEvent++, AudioObjectList[ObjAddr]->isKeyPressed, AudioObjectList[ObjAddr]->releaseStartingPoint, &AudioObjectList[ObjAddr]->toBeDeleted);
      totalWaveVal += (sampleVal * ampl);
    }
    totalWaveVal /= 850;
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


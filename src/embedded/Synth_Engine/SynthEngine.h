#ifndef SynthEngine_h
#define SynthEngine_h
#define Sample_Rate 40000
#include "Arduino.h"
#include "Envelope.h"
#include "Sound.h"
#include "AudioObject.h"

#define MAX_AUDIO_OBJECT_PER_CHANNEL 44
#define MAX_AUDIO_OBJECT_CHANNEL 3
#define MAX_AUDIO_ACTIVE_OBJECT_NUMBER 44
#define MAX_SOUNDS_NUMBER MAX_AUDIO_OBJECT_CHANNEL



// TaskHandle_t Core0;
#define Wavetable_Length 200
#define MaxWaveTypes 5
#define Wavetable_MaxAmplitude_val 255


#define WAVETYPE_SIN 0
#define WAVETYPE_SAW 2
#define WAVETYPE_TRIANG 1
#define WAVETYPE_SQUARE 3
#define WAVETYPE_SIN_3 4

#define MAGIC_BUFFER_OFFSET 100



extern byte wave[MAGIC_BUFFER_OFFSET * 2];
extern bool newSampleREQ;
extern bool newSampleREQ_SectionToFill;
extern unsigned int OutBufferIndex;

class SynthEngine {
private:
  const int _MAX_AUDIO_OBJECT_NUMBER = MAX_AUDIO_ACTIVE_OBJECT_NUMBER;
  int _currentlyPlayingNote;

public:
  SynthEngine();
  AudioObject* AudioObjectList[MAX_AUDIO_OBJECT_PER_CHANNEL * MAX_AUDIO_OBJECT_CHANNEL];
  Sound soundList[MAX_SOUNDS_NUMBER];
  bool start(int note, int channel);
  bool release(int note, int channel);
  bool stop(int id);
  int find_inActiveNoteList(int id);  // return Position in activeNoteList[]
  void cleanSilentObjects();
  int getActiveNotesNumber();
  void stopAll();
  int activeNoteList[MAX_AUDIO_ACTIVE_OBJECT_NUMBER];  //contains all active AudioObject's ids
  int ObjectChannelShift[MAX_AUDIO_OBJECT_CHANNEL] = { 24, 29, 41 };
  int getNoteId(int note, int channel);
  void Wavetable_reloadTable();

  void setupCore0();
  void generateAudioChunk(int a, bool b);
  void AudioCompositorHandler();

  byte Wavetable_table[MaxWaveTypes][Wavetable_Length];
  unsigned int FillBufferIndex = 0;
  // void setupTimerInterrupt();
};

void IRAM_ATTR IntSR();

#endif
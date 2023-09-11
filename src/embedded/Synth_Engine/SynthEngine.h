#ifndef SynthEngine_h
#define SynthEngine_h
#include "Arduino.h"
#include "Definitions.h"
#include "Envelope.h"
#include "Sound.h"
#include "AudioObject.h"

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
  bool stop_byActiveNoteListPos(int locat);
  int find_inActiveNoteList(int id);  // return Position in activeNoteList[]
  void cleanSilentObjects();
  int getActiveNotesNumber();
  void stopAll();
  int activeNoteList[MAX_AUDIO_ACTIVE_OBJECT_NUMBER];  //contains all active AudioObject's ids
  int ObjectChannelShift[MAX_AUDIO_OBJECT_CHANNEL] = { 24, 29, 41 };
  int getNoteId(int note, int channel);
  void reloadWavetable();

  void setupCore0();
  void generateAudioChunk(int a, bool b);
  void AudioCompositorHandler();

  byte Wavetable_table[MaxWaveTypes][Wavetable_Length];
  unsigned int FillBufferIndex = 0;
  bool activeNoteList_Change_REQ=false;
  bool activeNoteList_Change_ACK=false;
};

void IRAM_ATTR IntSR();

#endif
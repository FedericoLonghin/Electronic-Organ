#ifndef SynthEngine_h
#define SynthEngine_h
#include "Arduino.h"
#include "Definitions.h"
#include "AudioObject.h"
#include "Envelope.h"
#include "Sound.h"

extern byte wave[MAGIC_BUFFER_OFFSET * 2];
extern bool newSampleREQ;
extern bool newSampleREQ_SectionToFill;
extern unsigned int OutBufferIndex;

class SynthEngine {
private:
  const int _MAX_AUDIO_OBJECT_NUMBER = MAX_AUDIO_ACTIVE_OBJECT_NUMBER;

  int _currentlyPlayingNote_total;
  int _currentlyPlayingNote_perChannel[AUDIO_OBJECT_CHANNEL];
  void calc_currentlyPlayingNote_total();


public:
  SynthEngine();
  AudioObject* AudioObjectList[MAX_AUDIO_OBJECT_PER_CHANNEL * AUDIO_OBJECT_CHANNEL];
  Sound soundList[MAX_SOUNDS_NUMBER];
  bool start(int note, int channel);
  bool release(int note, int channel);
  bool stop(int note, int channel);
  int find_inActiveNoteList(int note, int channel);  // return Position in activeNoteList[]
  void cleanSilentObjects();
  int getActiveNotesNumber(int channel);
  void stopAll();
  int NewactiveNoteList[AUDIO_OBJECT_CHANNEL][MAX_AUDIO_ACTIVE_OBJECT_NUMBER];  //contains all active AudioObject's ids
  int ObjectChannelShift[AUDIO_OBJECT_CHANNEL] = { 24, 29, 41 };
  int getNoteId(int note, int channel);
  void reloadWavetable();

  void setupCore0();
  void generateAudioChunk(int a, bool b);
  void AudioCompositorHandler();

  byte Wavetable_table[MaxWaveTypes][Wavetable_Length];
  unsigned int FillBufferIndex = 0;
  bool activeNoteList_Change_REQ = false;
  bool activeNoteList_Change_ACK = false;

};

void IRAM_ATTR IntSR();

#endif
#include "driver/adc.h"
#define Wavetable_Length 200
#define Sample_Rate 40000
#define MAGIC_BUFFER_OFFSET 100

#define MaxWaveTypes 5
#define WAVETYPE_SIN 0
#define WAVETYPE_SAW 2
#define WAVETYPE_TRIANG 1
#define WAVETYPE_SQUARE 3
#define WAVETYPE_SIN_3 4

byte wave[MAGIC_BUFFER_OFFSET * 2];
unsigned int OutBufferIndex = 0;
unsigned int FillBufferIndex = 0;
bool newSampleREQ = false;
bool newSampleREQ_SectionToFill = false;

//Faster millis() alternative

#define Envelope_AttackDecay_Table_Length 10000
#define Envelope_Release_Table_Length 5000
class Envelope {
public:
  int Env_At;
  int Env_Al;
  int Env_Dt;
  int Env_Sl;
  int Env_Rt;
  int Env_R_completeTableLength = 0;
  // private:
  byte attackDecayTable[Envelope_AttackDecay_Table_Length];
  byte completeReleaseTable[Envelope_Release_Table_Length];
  unsigned int ReverseAttackTable[256];
  unsigned int ReverseCompleteReleaseTable[256];

public:
  int getAmplitude(unsigned long noteLife, bool isKeyPressed, unsigned int releaseStartingPoint, bool *toBeDeleted);
  void reloadTable();
  int getReleaseIndex(int val);
  int getAttackIndex(int val);
};

class Sound {
public:
  Envelope ADSR;
  byte Wavetype;
  void setDefaultParam() {

    ADSR.Env_At = 100;
    ADSR.Env_Al = 255;
    ADSR.Env_Dt = 100;
    ADSR.Env_Sl = 200;
    ADSR.Env_Rt = 200;
    ADSR.reloadTable();
    Wavetype = 0;
  }
};

#define MAX_AUDIO_OBJECT_PER_CHANNEL 44
#define MAX_AUDIO_OBJECT_CHANNEL 3
#define MAX_AUDIO_ACTIVE_OBJECT_NUMBER 44
#define MAX_SOUNDS_NUMBER MAX_AUDIO_OBJECT_CHANNEL
class AudioObject;

class SynthEngine {
private:
  static const int _MAX_AUDIO_OBJECT_NUMBER = MAX_AUDIO_ACTIVE_OBJECT_NUMBER;
  int _currentlyPlayingNote = 0;

public:
  AudioObject *AudioObjectList[MAX_AUDIO_OBJECT_PER_CHANNEL * MAX_AUDIO_OBJECT_CHANNEL];
  Sound soundList[MAX_SOUNDS_NUMBER];
  SynthEngine();
  bool start(int note, int channel);
  bool release(int note, int channel);
  bool stop(int id);
  int find_inActiveNoteList(int id);  // return Position in activeNoteList[]
  void cleanSilentObjects();
  int getActiveNotesNumber();
  void stopAll();
  int activeNoteList[MAX_AUDIO_ACTIVE_OBJECT_NUMBER] = { 0 };  //contains all active AudioObject's ids
  int ObjectChannelShift[MAX_AUDIO_OBJECT_CHANNEL] = { 24, 29, 41 };
};

SynthEngine audioEngine;

byte Wavetable_waveForm = WAVETYPE_SIN_3;
#define Wavetable_MaxAmplitude_val 255
byte Wavetable_table[MaxWaveTypes][Wavetable_Length];
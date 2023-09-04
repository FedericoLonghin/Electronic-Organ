#define Sample_num 112
#define Sample_rate 10000

#define MaxWaveTypes 4
#define WAVETYPE_SIN 0
#define WAVETYPE_SAW 2
#define WAVETYPE_TRIANG 1
#define WAVETYPE_SQURE 3

byte wave[Sample_rate];
int OutBufferIndex = 0;
int FillBufferIndex = 0;

#define MAX_AUDIO_OBJECT_NUMBER 20

class AudioObject;

class AudioObjectListMenager {
private:
  static const int _MAX_AUDIO_OBJECT_NUMBER = 20;
  int _currentlyPlayingNote;
public:

  AudioObject *AudioObjectList[_MAX_AUDIO_OBJECT_NUMBER];
  AudioObjectListMenager();
  bool add(int id);
  bool remove(int id);
  int find(int id);
  int getActiveNotesNumber();
  int fastNoteArray[_MAX_AUDIO_OBJECT_NUMBER] = { 0 };
};

AudioObjectListMenager AudioEngine;
#define Envelope_AttackDecay_Table_Length 10000
#define Envelope_Release_Table_Length 1000
class Envelope {
public:
  // int Env_A_t = 5000;
  // float Env_A_l = 1;
  // int Env_D_t = 500;
  // float Env_S_l = 0.0f;
  // int Env_R_t = 1000;

  int Env_At = 100;
  float Env_Al = 1;
  int Env_Dt = 1000;
  float Env_Sl = 0.0f;
  int Env_Rt = 1000;

  float getAmplitude(unsigned long eventTime, bool isKeyPressed);
  int getAmplitudeInt(unsigned long eventTime, bool isKeyPressed);
  float oldgetAmplitude(unsigned long eventTime, bool isKeyPressed);
  void reloadEnvelopeTable();
  float attackDecayTable[Envelope_AttackDecay_Table_Length];
  int attackDecayTableInt[Envelope_AttackDecay_Table_Length];
  float releaseTable[Envelope_Release_Table_Length];
};
class Sound {
  Envelope Env;
  int Volume;
  int WaveType;
};
unsigned long startnote;


Envelope env;
unsigned long myMillis = 0;

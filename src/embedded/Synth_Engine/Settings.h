#define Wavetable_Length 200
#define Sample_Rate 40000
#define MAGIC_BUFFER_OFFSET 200

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
  int Env_At = 100;
  int Env_Al = 255;
  int Env_Dt = 200;
  int Env_Sl = 255;
  int Env_Rt = 400;
  int Env_R_completeTableLength = 0;
  float Env_ACoeff = 1.1f;

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

#define MAX_AUDIO_OBJECT_NUMBER 40
class AudioObject;

class AudioObjectListMenager {
private:
  static const int _MAX_AUDIO_OBJECT_NUMBER = MAX_AUDIO_OBJECT_NUMBER;
  int _currentlyPlayingNote = 0;

public:
  AudioObject *AudioObjectList[_MAX_AUDIO_OBJECT_NUMBER];
  AudioObjectListMenager();
  bool start(int id);
  bool release(int id);
  bool stop(int id);
  int find(int id);
  void cleanSilentObjects();
  int getActiveNotesNumber();
  void stopAll();
  int fastNoteArray[_MAX_AUDIO_OBJECT_NUMBER] = { 0 };
};

AudioObjectListMenager AudioEngine;

class Sound {
public:
  Envelope Env;
  int Volume;
  int WaveType;
};


Envelope env;


byte Wavetable_waveForm = WAVETYPE_SIN_3;
#define Wavetable_MaxAmplitude_val 255
byte Wavetable_table[MaxWaveTypes][Wavetable_Length];
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
class Envelope {
public:
  int Env_A_t=1000;
  float Env_A_l=1;
  int Env_D_t=500;
  float Env_S_l=0.1f;
  int Env_R_t=1000;
  float getAmplitude(unsigned long startTime);
};
class Sound {
  Envelope Env;
  int Volume;
  int WaveType;
};
Envelope env;
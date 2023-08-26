#define Sample_num 112
#define Sample_rate 10000
#define MaxWaveTypes 4

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
  int getFrequency(int id);
  int getActiveNotesNumber();
int newValArray[20]={0};

};

AudioObjectListMenager AudioEngine;


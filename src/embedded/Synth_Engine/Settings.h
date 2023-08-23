#define Sample_num 112
#define Sample_rate 10000
#define MaxWaveTypes 4

byte wave[Sample_rate];
int OutBufferIndex = 0;
int FillBufferIndex = 0;

int freqToPlay = 1;
int freqToPlayList[] = { 440, 50, 1000 };

double intPart;
int nextMillis = 0;
double molt;
int val;
int totalWaveVal;

//Aud


#define MAX_AUDIO_OBJECT_NUMBER 10
int currentAudioObjectsNumber = 0;

class AudioObject;

AudioObject *AudioObjectList[MAX_AUDIO_OBJECT_NUMBER];
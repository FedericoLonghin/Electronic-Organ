#define Sample_num 112
#define Sample_rate 10000
int freq = 0;
#define MaxWaveTypes 4
int i = 0;
int k = 0;
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
TaskHandle_t Core0;

void setupCore0() {
  xTaskCreatePinnedToCore(
    Core0_SR,
    "Core0",
    10000,
    NULL,
    0,
    &Core0,
    0);
}

//Core 0 Service Routine
void Core0_SR(void* parameter) {
  for (;;) {
    generateAudioChunk(1000);
  }
}
//debug
//end debug






//                      |test 15 version|
#define S_rate 10000
#define S_n 112
#define freq 440
unsigned long val;
int totalWaveVal;
float divider=S_n/(float)S_rate;

void generateAudioChunk(int len) {
  int noteNum = AudioEngine.getActiveNotesNumber();
  for (int a = 0; a < len; a++) {
    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
        val = ((AudioEngine.newValArray[f] * FillBufferIndex) % (S_rate))*divider;

      totalWaveVal += WaveFormTable[2][val];
    }
    if (noteNum > 0) {
      totalWaveVal /= noteNum;
    }
    wave[FillBufferIndex] = totalWaveVal;
    FillBufferIndex++;
    if (FillBufferIndex >= Sample_rate) {

      FillBufferIndex = 0;
      tick();
    }
  }
}

//                      |test 14 version|
// double intPart;
// int val;
// int totalWaveVal;
// void generateAudioChunk(int len) {
//   int noteNum = AudioEngine.getActiveNotesNumber();
//   for (int a = 0; a < len; a++) {
//     totalWaveVal = 0;
//     for (int f = 0; f < noteNum; f++) {
//       val = (modf(((FillBufferIndex * AudioEngine.newValArray[0]) ), &intPart)) * Sample_num;
//       totalWaveVal += WaveFormTable[2][val];
//     }
//     if (noteNum > 0) {
//       totalWaveVal /= noteNum;
//     }
//     wave[FillBufferIndex] = 0;
//     FillBufferIndex++;
//     if (FillBufferIndex >= Sample_rate) {

//       FillBufferIndex = 0;
//       tick();
//     }
//   }
// }



//                      |test 13 version|
// double intPart;
// int val;
// int totalWaveVal;
// void generateAudioChunk(int len) {
//   int noteNum = AudioEngine.getActiveNotesNumber();
//   for (int a = 0; a < len; a++) {
//     totalWaveVal = 0;
//     for (int f = 0; f < noteNum; f++) {
//       val = (modf(((FillBufferIndex * AudioEngine.newValArray[0]) / (float)Sample_rate), &intPart)) * Sample_num;

//       totalWaveVal += WaveFormTable[2][val];
//     }
//     if (noteNum > 0) {
//       totalWaveVal /= noteNum;
//     }
//     wave[FillBufferIndex] = totalWaveVal;
//     FillBufferIndex++;
//     if (FillBufferIndex >= Sample_rate) {

//       FillBufferIndex = 0;
//       tick();
//     }
//   }
// }
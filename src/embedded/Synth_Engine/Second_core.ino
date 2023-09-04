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






#define S_rate 10000
#define S_n 112
#define freq 440
//                      |test 33 version|
unsigned long val;
unsigned long envelopedVal;
int totalWaveVal;
float divider = S_n / (float)(S_rate);
float ampl;

void generateAudioChunk(int len) {
  int noteNum = AudioEngine.getActiveNotesNumber();
  float noteAmpl[noteNum] = { 0 };
  int amp = 0;
  
  for (int a = 0; a < len; a++) {
    if (1 || FillBufferIndex % 50 == 0) {
      // for (int f = 0; f < noteNum; f++) {

      //   //noteAmpl[f] = env.getAmplitude(AudioEngine.AudioObjectList[f]->eventTime,AudioEngine.AudioObjectList[f]->isKeyPressed);
      //   // Serial.println(noteAmpl[f]);
      // }
    }
    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      val = ((AudioEngine.fastNoteArray[f] * FillBufferIndex) % (S_rate)) * divider;
      // val = 0;
       totalWaveVal += (WaveFormTable[WaveType][val] * env.getAmplitudeInt(AudioEngine.AudioObjectList[f]->eventTime, AudioEngine.AudioObjectList[f]->isKeyPressed));
      // totalWaveVal += (WaveFormTable[WaveType][val] * env.getAmplitudeInt(startnote, true));
      //  Serial.printf("%d: %d\n",f,env.getAmplitude(AudioEngine.AudioObjectList[f]->eventTime, AudioEngine.AudioObjectList[f]->isKeyPressed));
      // envelopedVal = (WaveFormTable[WaveType][val] * env.attackDecayTableInt[myMillis-startnote]);
      // totalWaveVal += envelopedVal;
    }

    totalWaveVal /= 765;
    // totalWaveVal *= 0.3f;
    wave[FillBufferIndex] = totalWaveVal < 255 ? totalWaveVal : 255;
    // Serial.print(0);
    // Serial.print(",");
    // Serial.print(255);
    // Serial.print(",");
    // Serial.println(wave[FillBufferIndex]);
    // wave[FillBufferIndex] = 0;
    FillBufferIndex++;
    if (FillBufferIndex >= Sample_rate) {

      FillBufferIndex = 0;
#ifdef SERIAL_DEBUG
      tick();
#endif
    }
  }
}



//                      |test 15-32 version|
// unsigned long val;
// int totalWaveVal;
// float divider = S_n / (float)(S_rate);
// float ampl;
// void generateAudioChunk(int len) {
//   int noteNum = AudioEngine.getActiveNotesNumber();
//   float noteAmpl[noteNum] = { 0 };
//   for (int a = 0; a < len; a++) {
//     if (1 || FillBufferIndex % 50 == 0) {
//       // for (int f = 0; f < noteNum; f++) {

//       //   //noteAmpl[f] = env.getAmplitude(AudioEngine.AudioObjectList[f]->eventTime,AudioEngine.AudioObjectList[f]->isKeyPressed);
//       //   // Serial.println(noteAmpl[f]);
//       // }
//     }
//     totalWaveVal = 0;
//     for (int f = 0; f < noteNum; f++) {
//       val = ((AudioEngine.fastNoteArray[f] * FillBufferIndex) % (S_rate)) * divider;
//       // val = 0;
//       totalWaveVal += (WaveFormTable[WaveType][val] * env.getAmplitude(AudioEngine.AudioObjectList[f]->eventTime, AudioEngine.AudioObjectList[f]->isKeyPressed));
//       // Serial.printf("%d: %f\n",f,totalWaveVal);
//       // totalWaveVal += (WaveFormTable[WaveType][val]);
//     }
//     // Serial.println();
//     if (noteNum > 0) {
//       // totalWaveVal /= noteNum;
//     }
//     totalWaveVal *= 0.3f;
//     wave[FillBufferIndex] = totalWaveVal < 255 ? totalWaveVal : 255;
//     FillBufferIndex++;
//     if (FillBufferIndex >= Sample_rate) {

//       FillBufferIndex = 0;
// #ifdef SERIAL_DEBUG
//       tick();
// #endif
//     }
//   }
// }

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
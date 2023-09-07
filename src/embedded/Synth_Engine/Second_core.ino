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
int c = 0;
//Core 0 Service Routine
void Core0_SR(void* parameter) {
  for (;;) {
    if (newSampleREQ) {
      newSampleREQ = false;
      generateAudioChunk(MAGIC_BUFFER_OFFSET);
    }
    c++;
    if (c > 10000) c = 0;
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
int divider_sp = 360;
int divider_pv = 360;
#define DIVIDER_INC_VAL 1
void generateAudioChunk(int len) {
  // Serial.println("Generated Audio Chunk");
  int noteNum = AudioEngine.getActiveNotesNumber();
  byte noteAmpl[noteNum] = { 0 };
  int amp = 0;
  for (int a = 0; a < len; a++) {

    if (FillBufferIndex % 1 == 0) {
      for (int f = 0; f < noteNum; f++) {

        noteAmpl[f] = env.getNewAmplitudeInt(AudioEngine.AudioObjectList[f]->ticksFromLastEvent++, AudioEngine.AudioObjectList[f]->isKeyPressed, AudioEngine.AudioObjectList[f]->releaseStartingPoint, &AudioEngine.AudioObjectList[f]->toBeDeleted);
        // noteAmpl[f] = env.getAmplitudeInt(AudioEngine.AudioObjectList[f]->eventTime++, AudioEngine.AudioObjectList[f]->isKeyPressed);
        // Serial.println(noteAmpl[f]);
      }
    }
    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      val = ((AudioEngine.fastNoteArray[f] * FillBufferIndex) % (S_rate)) * divider;
      // val = 0;
      totalWaveVal += (WaveFormTable[WaveType][val] * noteAmpl[f]);
      // totalWaveVal += (WaveFormTable[WaveType][val] *env.getAmplitudeInt(AudioEngine.AudioObjectList[f]->eventTime, AudioEngine.AudioObjectList[f]->isKeyPressed) );

      // totalWaveVal += (WaveFormTable[WaveType][val] * env.getAmplitudeInt(startnote, true));
      // Serial.printf("%d: %d\n",f,env.getAmplitudeInt(AudioEngine.AudioObjectList[f]->eventTime, AudioEngine.AudioObjectList[f]->isKeyPressed));
      // envelopedVal = (WaveFormTable[WaveType][val] * env.attackDecayTableInt[myMillis-startnote]);
      // totalWaveVal += envelopedVal;
    }

    // divider_sp = 360 * noteNum;
    // if (noteNum > 0) {
    //   if (divider_pv > divider_sp) divider_pv -= DIVIDER_INC_VAL;
    //   else if (divider_pv < divider_sp) divider_pv += DIVIDER_INC_VAL;
    //   totalWaveVal /= divider_pv;
    //   // Serial.printf("sp:%d\tpv:%d\n", divider_sp, divider_pv);
    // }


     totalWaveVal /= 850;
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
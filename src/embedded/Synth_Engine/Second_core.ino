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
    generateAudioChunk(100);
  }
}

void generateAudioChunk(int len) {
  for (int a = 0; a < len; a++) {
    totalWaveVal = 0;
    for (int f = 0; f < freqToPlay; f++) {
      molt = modf((FillBufferIndex * freqToPlayList[f] / (double)Sample_rate), &intPart);
      val = molt * Sample_num;
      totalWaveVal += WaveFormTable[0][val];
    }
    totalWaveVal /= freqToPlay;
    wave[FillBufferIndex] = totalWaveVal;
    FillBufferIndex++;
    if (FillBufferIndex >= Sample_rate) FillBufferIndex = 0;
  }
}
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
int noteNum=AudioEngine->currentlyPlayingNote;
  for (int a = 0; a < len; a++) {
    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      molt = modf((FillBufferIndex * AudioEngine->getFrequency(f) / (double)Sample_rate), &intPart);
      val = molt * Sample_num;
      totalWaveVal += WaveFormTable[0][val];
    }
    if (noteNum > 0) {
      totalWaveVal /= noteNum;
    }
    wave[FillBufferIndex] = totalWaveVal;
    FillBufferIndex++;
    if (FillBufferIndex >= Sample_rate) FillBufferIndex = 0;
  }
}
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
      generateAudioChunk(MAGIC_BUFFER_OFFSET, newSampleREQ_SectionToFill);
    }
    c++;  //just for triggering WDT
  }
}

unsigned long val;
unsigned int totalWaveVal;
float divider = Wavetable_Length / (float)(Sample_Rate);

void generateAudioChunk(int len, bool _section) {
  int noteNum = AudioEngine.getActiveNotesNumber();
  byte noteAmpl[noteNum] = { 0 };
  for (int a = 0; a < len; a++) {

    if (FillBufferIndex % 1 == 0) {
      for (int f = 0; f < noteNum; f++) {

        noteAmpl[f] = env.getAmplitude(AudioEngine.AudioObjectList[f]->ticksFromLastEvent++, AudioEngine.AudioObjectList[f]->isKeyPressed, AudioEngine.AudioObjectList[f]->releaseStartingPoint, &AudioEngine.AudioObjectList[f]->toBeDeleted);
      }
    }
    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      val = ((AudioEngine.fastNoteArray[f] * FillBufferIndex) % (Sample_Rate)) * divider;
      totalWaveVal += (Wavetable_table[Wavetable_waveForm][val] * noteAmpl[f]);
      // totalWaveVal += (WaveFormTable[WaveType][val] * noteAmpl[f]);
    }




    totalWaveVal /= 850;
    wave[MAGIC_BUFFER_OFFSET * _section + a] = totalWaveVal < 255 ? totalWaveVal : 255;

    FillBufferIndex++;
    if (FillBufferIndex >= Sample_Rate) {
      FillBufferIndex = 0;
    }
  }
}

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

byte sampleVal;
byte ampl;
unsigned int totalWaveVal;
float divider = Wavetable_Length / (float)(Sample_Rate);
void generateAudioChunk(int len, bool _section) {
  int noteNum = AudioEngine.getActiveNotesNumber();
  for (int a = 0; a < len; a++) {

    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      int ObjAddr = AudioEngine.activeNoteList[f];
      sampleVal = Wavetable_table[Wavetable_waveForm][(byte)((((AudioEngine.AudioObjectList[ObjAddr]->frequency) * FillBufferIndex) % (Sample_Rate)) * divider)];
      ampl = env.getAmplitude(AudioEngine.AudioObjectList[ObjAddr]->ticksFromLastEvent++, AudioEngine.AudioObjectList[ObjAddr]->isKeyPressed, AudioEngine.AudioObjectList[ObjAddr]->releaseStartingPoint, &AudioEngine.AudioObjectList[ObjAddr]->toBeDeleted);
      totalWaveVal += (sampleVal * ampl);
    }
    totalWaveVal /= 850;
    wave[MAGIC_BUFFER_OFFSET * _section + a] = totalWaveVal < 255 ? totalWaveVal : 255;

    FillBufferIndex++;
    if (FillBufferIndex >= Sample_Rate) {
      FillBufferIndex = 0;
    }
  }
}

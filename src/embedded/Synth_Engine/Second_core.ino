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
  int noteNum = audioEngine.getActiveNotesNumber();
  for (int a = 0; a < len; a++) {

    totalWaveVal = 0;
    for (int f = 0; f < noteNum; f++) {
      int ObjAddr = audioEngine.activeNoteList[f];
      sampleVal = Wavetable_table[audioEngine.soundList[audioEngine.AudioObjectList[ObjAddr]->sound].Wavetype][(byte)((((audioEngine.AudioObjectList[ObjAddr]->frequency) * FillBufferIndex) % (Sample_Rate)) * divider)];
      ampl = audioEngine.soundList[audioEngine.AudioObjectList[ObjAddr]->sound].ADSR.getAmplitude(audioEngine.AudioObjectList[ObjAddr]->ticksFromLastEvent++, audioEngine.AudioObjectList[ObjAddr]->isKeyPressed, audioEngine.AudioObjectList[ObjAddr]->releaseStartingPoint, &audioEngine.AudioObjectList[ObjAddr]->toBeDeleted);
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

#include "Settings.h"
#include "Waveform.h"



void setup() {
  Serial.begin(115200);
  setupTimerInterrupt();
  setupCore0();
}





void loop() {
  // noteOn(50);
  // delay(1000);
  noteOn(36);
  // noteOn(329);
  // noteOn(392);

  // delay(1000);
  // noteOn(1000);
  // for (int i = 0; i < currentAudioObjectsNumber; i++) {
  //   Serial.printf("Obj n. %d: freq: %d ", i, AudioObjectList[i]->frequency);
  // }
  for (;;) {

  }
}

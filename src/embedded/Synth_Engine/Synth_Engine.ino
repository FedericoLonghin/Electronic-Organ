#include "SynthEngine.h"

#define SERIAL_DEBUG 1

SynthEngine audioEngine;
TaskHandle_t Core0;

void setupTimerInterrupt();
void setup() {
  xTaskCreatePinnedToCore(Compositor_SR, "Core0", 10000, NULL, 0, &Core0, 0);
  setCpuFrequencyMhz(2400);
  if (SERIAL_DEBUG) Serial.begin(921600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  // setupTimerInterrupt();
  // setupCore0();
  setupTimerInterrupt();
  Serial.printf("Electronic Organ started!\nCPU Freq: %d\n", getCpuFrequencyMhz());

  audioEngine.reloadWavetable();
  audioEngine.soundList[0].setDefaultParam();
  audioEngine.soundList[1].setDefaultParam();
  audioEngine.soundList[2].setDefaultParam();
  audioEngine.soundList[2].Wavetype = 2;
  audioEngine.soundList[1].Wavetype = WAVETYPE_SIN_3;
  audioEngine.soundList[1].Trem.enable = 1;
}

void loop() {
  int msg_len_i = 0;
  bool payload = false;
  String msg = "";
  if (Serial2.available()) {
    msg = Serial2.readStringUntil('.');
    payload = true;
    msg_len_i++;
  }
  if (payload) {
    requestAction(msg);
    payload = false;
  }
  audioEngine.cleanSilentObjects();
}

void Compositor_SR(void* parameter) {
  audioEngine.AudioCompositorHandler();
}
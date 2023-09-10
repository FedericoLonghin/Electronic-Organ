#include "Settings.h"
#include "Waveform.h"
#define SERIAL_DEBUG 1
#define WaveType WAVETYPE_SIN
void setup() {
  setCpuFrequencyMhz(2400);
  if (SERIAL_DEBUG) Serial.begin(921600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  setupTimerInterrupt();
  setupCore0();
  Serial.printf("CPU Freq: %d\n", getCpuFrequencyMhz());
  Wavetable_reloadTable();
  audioEngine.soundList[0].setDefaultParam();
  audioEngine.soundList[1].setDefaultParam();
  audioEngine.soundList[2].setDefaultParam();
  audioEngine.soundList[2].Wavetype=2;
  audioEngine.soundList[1].Wavetype=WAVETYPE_SIN_3;
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

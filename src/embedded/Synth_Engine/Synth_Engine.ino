#include "Settings.h"
#include "Waveform.h"
#define WaveType SINE_WAVE
void setup() {
  setCpuFrequencyMhz(2400);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  setupTimerInterrupt();
  setupCore0();
  Serial.printf("CPU Freq: %d\n", getCpuFrequencyMhz());
}
#define MESSAGE_FIXED_LENGTH 10
void loop() {

  char* msg_char = "";
  int msg_len_i = 0;
  bool payload = false;
  String msg = "";
  if (Serial2.available()) {
    msg = Serial2.readStringUntil('.');
    payload = true;
    msg_len_i++;
    // delay(5);
  }

  if (payload) {

    if (msg.startsWith("N-On")) {
      startCounter(100, false);
      int note = msg.substring(5).toInt();
      Serial.printf("Playing note %d\n", note);
      AudioEngine.add(note);
      

    } else if (msg.startsWith("N-Off")) {
      int note = msg.substring(6).toInt();
      Serial.printf("Stopping note %d\n", note);
      AudioEngine.remove(note);
    }
    payload = false;
  }
}

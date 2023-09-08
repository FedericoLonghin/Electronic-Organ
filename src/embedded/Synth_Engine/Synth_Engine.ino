#include "Settings.h"
#include "Waveform.h"
#define SERIAL_DEBUG
#define WaveType WAVETYPE_SIN
void setup() {
  setCpuFrequencyMhz(2400);
#ifdef SERIAL_DEBUG
  Serial.begin(921600);
#endif
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  setupTimerInterrupt();
  setupCore0();
  Serial.printf("CPU Freq: %d\n", getCpuFrequencyMhz());
  env.reloadTable();
  Wavetable_reloadTable();
  for (int i = 0; i < Wavetable_Length; i++) {
    Serial.println(Wavetable_table[Wavetable_waveForm][i]);
  }
  // tuningReq();
}

void loop() {
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
      int note = msg.substring(5).toInt();

#ifdef SERIAL_DEBUG
      Serial.printf("Playing note %d\n", note);
#endif
      AudioEngine.start(note);

    } else if (msg.startsWith("N-Off")) {
      int note = msg.substring(6).toInt();

#ifdef SERIAL_DEBUG
      Serial.printf("Stopping note %d\n", note);
#endif

      AudioEngine.release(note);

    } else if (msg.startsWith("CC")) {
      Serial.println(msg);
      byte control = msg.substring(3, 6).toInt();
      byte value = msg.substring(6, 10).toInt();
#ifdef SERIAL_DEBUG
      Serial.printf("Control Change Message: %d %d\n", control, value);
#endif
      if (control == 24) tuningReq();
    }
    payload = false;
  }
  AudioEngine.cleanSilentObjects();
}

void tuningReq() {
  AudioEngine.stopAll();
  Serial.println("TuneREQ.");
  Serial2.println("TuneREQ 0.");
}

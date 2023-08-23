#include "Settings.h"
#include "Waveform.h"



void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  setupTimerInterrupt();
  setupCore0();

  AudioEngine = new AudioObjectListNew(5);
  // AudioEngine->add(45);
  // AudioEngine->add(48);
  // Serial.printf("AOL find: %d", AudioEngine->find(48));
  // delay(1000);
  // AudioEngine->remove(48);
}



// void loop() { //Choose Serial1 or Serial2 as required
//   while (Serial2.available()) {
//     Serial.print(char(Serial2.read()));
//   }
//   // Serial.println("no");
// }

void loop() {
  String msg = "";
  bool payload = false;
  while (Serial2.available()) {
    msg += (char)Serial2.read();
    payload = true;
    delay(5);
  }
  if (payload) {
    Serial.println(msg);
    if (msg.startsWith("N-On")) {
      // Serial.print("OK");
      int note = msg.substring(5).toInt();
      Serial.printf("Playing note %d\n", note);
      AudioEngine->add(note);

    } else if (msg.startsWith("N-Off")) {
      // Serial.print("OK");
      int note = msg.substring(6).toInt();
      Serial.printf("Stopping note %d\n", note);
      AudioEngine->remove(note);
    }

    //   }
    //   // Serial.println("AA");
    // // }
    payload = false;
  }
}

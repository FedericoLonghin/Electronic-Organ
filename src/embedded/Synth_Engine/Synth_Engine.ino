#include "Settings.h"
#include "Waveform.h"



void setup() {
  Serial.begin(115200);
  setupTimerInterrupt();
  setupCore0();

  AudioEngine = new AudioObjectListNew(5);
  // AudioEngine->add(45);
  // AudioEngine->add(48);
  // Serial.printf("AOL find: %d", AudioEngine->find(48));
  // delay(1000);
  // AudioEngine->remove(48);
}





void loop() {
  String msg = "";
  bool payload = false;
  while (Serial.available()) {
    msg += (char)Serial.read();
    payload = true;
  }
  if (payload) {
Serial.println("got something");
Serial.println(msg);
    if (msg.startsWith("Non")) {
      // Serial.print("OK");
      int note = msg.substring(4).toInt();
      Serial.printf("Playing note %d\n", note);
      AudioEngine->add(note);

    } else if (msg.startsWith("Noff")) {
      // Serial.print("OK");
      int note = msg.substring(5).toInt();
      Serial.printf("Playing note %d\n", note);
      AudioEngine->remove(note);
    }

    //   }
    //   // Serial.println("AA");
    // // }
    payload=false;
  }
}

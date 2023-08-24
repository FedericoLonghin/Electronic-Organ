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
#define MESSAGE_FIXED_LENGTH 10
void loop() {
  if (Serial.available()) {
    // Serial.println("got so");
    // Serial.println(Serial.readString());
    if (Serial.readString().startsWith("stop")) {
      Serial.println("Stopping all sounds");
      AudioEngine->currentlyPlayingNote = 0;
    }
  }
  char* msg_char="";
  int msg_len_i = 0;
  bool payload = false;
  String msg="";
  if(Serial2.available()){

  //while ( msg_len_i < MESSAGE_FIXED_LENGTH) {

    // msg += (char)
    // Serial2.readBytes(msg_char,1);
     msg=Serial2.readStringUntil('.');
    payload = true;
    msg_len_i++;
    delay(5);
  // }
  }

  // if (Serial2.available()) {
  //   msg += (char)Serial2.read();
  //   payload = true;
  //   delay(5);
  // }
  if (payload) {
    
    Serial.println("New Message:");
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

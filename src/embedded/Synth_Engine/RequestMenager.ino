void requestAction(String msg) {
  if (msg.startsWith("N-On")) {
    Serial.println(msg);
    int channel = msg.substring(5, 8).toInt();
    int note = msg.substring(9, 12).toInt();

    if (SERIAL_DEBUG) Serial.printf("Playing ch: %d at note: %d\n", channel, note);

    audioEngine.start(note, channel);

  } else if (msg.startsWith("N-Off")) {
    int channel = msg.substring(6, 9).toInt();
    int note = msg.substring(10, 13).toInt();
    if (SERIAL_DEBUG) Serial.printf("Releasing ch: %d at note: %d\n", channel, note);


    audioEngine.release(note, channel);

  } else if (msg.startsWith("CC")) {
    Serial.println(msg);
    byte control = msg.substring(3, 6).toInt();
    byte value = msg.substring(7, 10).toInt();
    if (SERIAL_DEBUG)
      Serial.printf("Control Change Message: %d %d\n", control, value);

    if (control == 30) {
      tuningReq();
    }
    //   else if (control == 32) {  //Percuss
    //   if (value == 0) env.Env_At = 200;
    //   else env.Env_At = 10;
    //   env.reloadTable();
    // } else if (control == 31) {  //Sustain
    //   if (value == 0) env.Env_Rt = 200;
    //   else env.Env_Rt = 600;
    //   env.reloadTable();
    // } else if (control >= 20 && control <= 24) {  //wavetype
    //   Wavetable_waveForm = control - 20;
    //   Wavetable_reloadTable();
    // }
  }
}



void tuningReq() {
  audioEngine.stopAll();
  Serial.println("TuneREQ.");
  Serial2.println("TuneREQ 0.");
}
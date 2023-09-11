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
    byte channel = msg.substring(3, 6).toInt();
    byte control = msg.substring(7, 10).toInt();
    byte value = msg.substring(11, 14).toInt();
    if (SERIAL_DEBUG) Serial.printf("Control Change Message: %d %d %d\n", channel, control, value);
    switch (channel) {
      case 0:
        if (control == 20 && value) audioEngine.soundList[0].Wavetype = WAVETYPE_TRIANG;
        else if (control == 21 && value) audioEngine.soundList[0].Wavetype = WAVETYPE_SIN;
        else if (control == 22) {
          audioEngine.soundList[0].ADSR.Env_Rt = value ? 600 : 200;
          audioEngine.soundList[0].ADSR.reloadTable();
        }
        break;
      case 1:
        if (control == 20 && value) audioEngine.soundList[1].Wavetype = WAVETYPE_SIN;
        else if (control == 21 && value) audioEngine.soundList[1].Wavetype = WAVETYPE_TRIANG;
        else if (control == 22 && value) audioEngine.soundList[1].Wavetype = WAVETYPE_SIN_3;
        else if (control == 23 && value) audioEngine.soundList[1].Wavetype = WAVETYPE_SAW;

        break;
      case 2:
        if (control == 20 && value) audioEngine.soundList[2].Wavetype = WAVETYPE_SIN;
        else if (control == 21 && value) audioEngine.soundList[2].Wavetype = WAVETYPE_TRIANG;
        else if (control == 22 && value) audioEngine.soundList[2].Wavetype = WAVETYPE_SAW;
        else if (control == 23 && value) audioEngine.soundList[2].Wavetype = WAVETYPE_SIN_3;
        else if (control == 24 && value) audioEngine.soundList[2].Wavetype = WAVETYPE_SQUARE;
        else if (control == 31) {
          audioEngine.soundList[2].ADSR.Env_Rt = value ? 600 : 200;
          audioEngine.soundList[2].ADSR.reloadTable();
        } else if (control == 32) {
          audioEngine.soundList[2].ADSR.Env_At = value ? 20 : 100;
          audioEngine.soundList[2].ADSR.reloadTable();
        }
        break;
      case 3:
        if (control == 29) {  // Space Motion
          audioEngine.soundList[1].EFX_tremolo_enable = value;
        } else if (control == 30) {  //Space Slow Fast
          audioEngine.soundList[1].EFX_tremolo_speed = value ? 2 : 10;
        }
        break;
    }
  }
}

void tuningReq() {
  audioEngine.stopAll();
  Serial.println("TuneREQ.");
  Serial2.println("TuneREQ 0.");
}
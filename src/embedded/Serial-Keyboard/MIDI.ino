void noteOn(byte channel, byte pitch, byte velocity) {
  // midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  // MidiUSB.sendMIDI(noteOn);
  String str = "N-On ";
  // str += channel;
  // str += " ";
  str += pitch;
  // str += " ";
  // str += velocity;
  // str += "\n";
  str=formatSerialMessage(str,10);
  Serial1.print(str);
    #ifdef SERIAL_DEBUG

  Serial.println(str);
      #endif
}

void noteOff(byte channel, byte pitch, byte velocity) {
  String str = "N-Off ";
  // str += channel;
  // str += " ";
  str += pitch;
  // str += " ";
  // str += velocity;
  // str += "\n";
  str=formatSerialMessage(str,10);
  Serial1.print(str);
    #ifdef SERIAL_DEBUG

  Serial.println(str);
    #endif
  // midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  // MidiUSB.sendMIDI(noteOff);
}

void programChange(byte channel, byte program) {  //deprecated
  // midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
  // MidiUSB.sendMIDI(pc);
}

void controlChange(byte channel, byte control, byte value) {  //deprecated

  // midiEventPacket_t event = {0x7b, 0xB0 | channel, control, value};
  // MidiUSB.sendMIDI(event);
}

String formatSerialMessage(String str, int len) {
  // if (str.length() == len) return str;
  // else if (str.length() > len) {
  //   return str.substring(len);
  // } else {
  //   while (str.length() < len) { str += "."; }
  //   return str;
  // }
  return str+='.';
}

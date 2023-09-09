void noteOn(byte channel, byte pitch, byte velocity) {
  String str = "N-On ";
  str += getFormattedNumber(channel, 3);
  str += " ";
  str += getFormattedNumber(pitch, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);

#ifdef SERIAL_DEBUG
  Serial.println(str);
#endif
}

void noteOff(byte channel, byte pitch, byte velocity) {
  String str = "N-Off ";
  str += getFormattedNumber(channel, 3);
  str += " ";
  str += getFormattedNumber(pitch, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);

#ifdef SERIAL_DEBUG
  Serial.println(str);
#endif
}

void programChange(byte channel, byte program) {  //deprecated
  // midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
  // MidiUSB.sendMIDI(pc);
}

void controlChange(byte channel, byte control, byte value) {  //deprecated
  String str = "CC ";
  str += getFormattedNumber(control, 3);
  str += " ";
  str += getFormattedNumber(value, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);

#ifdef SERIAL_DEBUG
  Serial.println(str);
#endif
}

String formatSerialMessage(String str, int len) {
  // if (str.length() == len) return str;
  // else if (str.length() > len) {
  //   return str.substring(len);
  // } else {
  //   while (str.length() < len) { str += "."; }
  //   return str;
  // }
  return str += '.';
}

String getFormattedNumber(byte num, byte final_length) {
  String str = (String)num;
  while (str.length() < final_length) { str = "0" + str; }
  return str;
}

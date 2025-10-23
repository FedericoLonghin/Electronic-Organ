#define message_len 8


void noteOn(byte channel, byte pitch, byte velocity) {
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 1);
#endif

#ifdef NEW_VERSION
  char *str = malloc(message_len + 1);
  memset(str, ' ', message_len);
  str[message_len] = 0;
  str[0] = 144 + channel * 0;
  str[1] = pitch + 53;
  str[2] = velocity;
  Serial1.print(str);
#else
  String str = "N-On ";
  str += getFormattedNumber(channel, 3);
  str += " ";
  str += getFormattedNumber(pitch, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);
#endif
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 0);
#endif
#ifdef SERIAL_DEBUG
  Serial.println("N-on");
#endif
}

void noteOff(byte channel, byte pitch, byte velocity) {
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 1);
#endif

#ifdef NEW_VERSION
  // char str[8];
  char *str = malloc(message_len + 1);
  memset(str, ' ', message_len);
  str[message_len] = 0;

  str[0] = 128 + channel * 0;
  str[1] = pitch + 53;
  str[2] = velocity;
  Serial1.print(str);
#else
  String str = "N-Off ";
  str += getFormattedNumber(channel, 3);
  str += " ";
  str += getFormattedNumber(pitch, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);
#endif
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 0);
#endif

#ifdef SERIAL_DEBUG
  Serial.println("N-Off");
#endif
}


void SendParam_float(byte cmd,byte param1,byte param2,byte param3, float val_float) {
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 1);
#endif
  // Serial.println(val_float);
  uint32_t *val = (uint32_t *)&val_float;
  // uint32_t val = 12345678;
  Serial.print((uint32_t)val);
  char *str = malloc(message_len + 1);
  memset(str, ' ', message_len);
  str[message_len] = 0;
  str[0] = cmd;
  str[1] = param1;
  str[2] = param2;
  str[3] = param3;
  // str[1] = ;
  str[4] = ((uint8_t)(*val >> 24));
  str[5] = ((uint8_t)(*val >> 16));
  str[6] = ((uint8_t)(*val >> 8));
  str[7] = ((uint8_t)(*val >> 0));
  Serial1.print(str);
  Serial.println(str[2]);


#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 0);
#endif

#ifdef SERIAL_DEBUG

#endif
}






void programChange(byte channel, byte program) {  //deprecated
  // midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
  // MidiUSB.sendMIDI(pc);
}

void controlChange(byte channel, byte control, byte value) {  //deprecated
#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 1);
#endif

  String str = "CC ";
  str += getFormattedNumber(channel, 3);
  str += " ";
  str += getFormattedNumber(control, 3);
  str += " ";
  str += getFormattedNumber(value, 3);
  str = formatSerialMessage(str, 10);
  Serial1.print(str);

#ifdef LED_DEBUG
  digitalWrite(DEBUG_LED_PIN, 0);
#endif

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

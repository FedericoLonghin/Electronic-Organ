

int Envelope::getAmplitudeInt(unsigned long eventTime, bool isKeyPressed) {
  // int noteLife_ms = eventTime;
  int noteLife_ms = myMillis - eventTime;
  if (isKeyPressed) {

    //if (noteLife_ms < Env_At) {
     if (noteLife_ms < Env_At + Env_Dt) {
      // Serial.println("a");
      return attackDecayTableInt[noteLife_ms];
    }
    return Env_Al;
  }
  return releaseTable[noteLife_ms];
}





float Envelope::getAmplitude(unsigned long eventTime, bool isKeyPressed) {
  int noteLife_ms = millis() - eventTime;
  if (isKeyPressed) {

    if (noteLife_ms < Env_At + Env_Dt) {
      return attackDecayTable[noteLife_ms];
    }
    return Env_Sl;
  }
  return releaseTable[noteLife_ms];
}
//old old function
// float Envelope::oldgetAmplitude(unsigned long eventTime, bool isKeyPressed) {
//   int noteLife = millis() - eventTime;
//   if (noteLife < Env_A_t) {
//     return noteLife * (Env_A_l / (float)Env_A_t);
//   }
//   if (noteLife < Env_A_t + Env_D_t) {
//     return Env_A_l + ((noteLife - Env_A_t) / (float)Env_D_t) * (Env_S_l - Env_A_l);
//   }


//   return Env_S_l;
// }


void Envelope::reloadEnvelopeTable() {


  //Attack
  for (int i = 0; i < Env_At; i++) {
    attackDecayTable[i] = (i / (float)Env_At) * Env_Al;
    attackDecayTableInt[i] = (i * 255) / Env_At;
  }
  // Decay
  for (int i = Env_At; i < Env_Dt + Env_At; i++) {
    attackDecayTable[i] = Env_Al - ((i - Env_At) / (float)Env_Dt) * (Env_Al - Env_Sl);
    attackDecayTableInt[i] = (Env_Al - ((i - Env_At) / (float)Env_Dt) * (Env_Al - Env_Sl)) * 255;
  }
  // Release
  for (int i = 0; i < Env_Rt; i++) {
    releaseTable[i] = Env_Sl - (i / (float)Env_Rt) * Env_Sl;
  }
}
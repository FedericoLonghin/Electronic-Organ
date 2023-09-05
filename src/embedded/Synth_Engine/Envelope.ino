

int Envelope::getAmplitudeInt(unsigned long eventTime, bool isKeyPressed) {
  // int noteLife_ms = eventTime;
  unsigned long noteLife_ms = currentTime_ms - eventTime;
  //Serial.printf("currenttime_ms:%d\teventTime:%d\tnoteLife:%d\tikp:%d\t", currentTime_ms,eventTime, noteLife_ms, isKeyPressed);
  if (isKeyPressed) {
    //if (noteLife_ms < Env_At) {
    if (noteLife_ms < Env_At + Env_Dt) {
      //Serial.println("A/D");
      return attackDecayTableInt[noteLife_ms];
    }
    //Serial.println("S");
    return Env_Sl;
  }
  if (noteLife_ms < Env_Rt)
    //Serial.println("R");
    return releaseTableInt[noteLife_ms];
  //Serial.println("END");
  return 0;
}




void Envelope::reloadEnvelopeTable() {


  //Attack
  if (env.Env_ALinear) {
    for (int i = 0; i < Env_At; i++) {
      attackDecayTableInt[i] = (i * Env_Al) / Env_At;
    }
  } else {
    int i;
    float val = 1.0;
    for (i = 0; val <= env.Env_Al; i++) {
      val *= env.Env_ACoeff;
      attackDecayTableInt[i] = (int)val;
    }
    env.Env_At = i - 1;
    env.Env_Al = attackDecayTableInt[1 - i];
  }

  // Decay
  for (int i = Env_At; i < Env_Dt + Env_At; i++) {
    attackDecayTableInt[i] = Env_Al - ((i - Env_At) / (float)Env_Dt) * (Env_Al - Env_Sl);
  }

  // Release
  for (int i = 0; i < Env_Rt; i++) {
    releaseTableInt[i] = (Env_Sl - (i / (float)Env_Rt) * Env_Sl);
  }
}
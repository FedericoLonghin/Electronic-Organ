int Envelope::getAmplitude(unsigned long noteLife, bool isKeyPressed, unsigned int releaseStartingPoint, bool *toBeDeleted) {
  noteLife /= (Sample_Rate / 1000);  //changing referement to millisecond
  if (isKeyPressed) {
    if (noteLife < Env_At + Env_Dt) {
      return attackDecayTable[noteLife];
    }
    return Env_Sl;
  }
  if (releaseStartingPoint + noteLife < Env_R_completeTableLength) {
    return completeReleaseTable[releaseStartingPoint + noteLife];
  }
  *toBeDeleted = true;
  return 0;
}

void Envelope::reloadTable() {
  //Attack
  if (Env_ALinear) {
    for (int i = 0; i < Env_At; i++) {
      attackDecayTable[i] = (i * Env_Al) / Env_At;
    }
  } else {
    int i;
    float val = 1.0;
    for (i = 0; val <= Env_Al; i++) {
      val *= Env_ACoeff;
      attackDecayTable[i] = (int)val;
    }
    Env_At = i - 1;
    Env_Al = attackDecayTable[i - 1];
  }

  // Decay
  for (int i = Env_At; i < Env_Dt + Env_At; i++) {
    attackDecayTable[i] = Env_Al - ((i - Env_At) / (float)Env_Dt) * (Env_Al - Env_Sl);
  }

  //Complete Release
  Env_R_completeTableLength = Env_Rt * 255 / Env_Sl;
  for (int i = 0; i < Env_R_completeTableLength; i++) {
    completeReleaseTable[i] = (255 - (i / (float)Env_R_completeTableLength) * 255);
  }

  //Reverse Complete Release
  for (int i = 0; i < 255; i++) {
    ReverseCompleteReleaseTable[i] = (255 - i) * Env_R_completeTableLength / 255;
  }
}

int Envelope::getReleaseIndex(int val) {
  return ReverseCompleteReleaseTable[val];
}


// int Envelope::getAmplitudeInt(unsigned long eventTime, bool isKeyPressed) {
//   // short noteLife_ms = currentTime_ms - eventTime;
//   if (isKeyPressed) {
//     // if (noteLife_ms < Env_At + Env_Dt) {
//       // return attackDecayTableInt[noteLife_ms];
//       if(fadeiter<Env_Al){
//     return fadeiter++;
//       }
//       else{
//         if(fadeiter>Env_Sl)
//          fadeiter--;
//       }
//       return fadeiter;
//     // }
//     return Env_Sl;
//   }
//   return 0;
// if (noteLife_ms < Env_Rt)
//   return releaseTableInt[noteLife_ms];
// return 0;
// }
// int Envelope::getAmplitudeInt(unsigned long eventTime, bool isKeyPressed) {
//   short noteLife_ms = currentTime_ms - eventTime;
//   if (isKeyPressed) {
//     if (noteLife_ms < Env_At + Env_Dt) {
//       return attackDecayTableInt[noteLife_ms];
//     }
//     return Env_Sl;
//   }
//   if (noteLife_ms < Env_Rt)
//     return releaseTableInt[noteLife_ms];
//   return 0;
// }
int Envelope::getAmplitudeInt(unsigned long eventTime, bool isKeyPressed) {
  unsigned short noteLife_ms = currentTime_ms - eventTime;
  if (isKeyPressed) {
    if (noteLife_ms < Env_At + Env_Dt) {
      // return 50;
      return attackDecayTableInt[noteLife_ms];
    }
    return Env_Sl;
  }
  if (noteLife_ms < Env_Rt)
    return releaseTableInt[noteLife_ms];
  return 0;
}



int Envelope::getNewAmplitudeInt(unsigned long noteLife, bool isKeyPressed, unsigned int releaseStartingPoint) {
  noteLife /= 10;  //changing referement to millisecond
  if (isKeyPressed) {
    if (noteLife < Env_At + Env_Dt) {
      // return 50;
      return attackDecayTableInt[noteLife];
    }
    return Env_Sl;
  }
  if (noteLife < Env_Rt)
    // return releaseTableInt[noteLife];
    // Serial.printf("relStartPoint:%d\tNotelife:%d\n",releaseStartingPoint,noteLife);
    return completeReleaseTableInt[releaseStartingPoint + noteLife];
  return 0;
}




void Envelope::reloadEnvelopeTable() {
  //Attack
  if (Env_ALinear) {
    for (int i = 0; i < Env_At; i++) {
      attackDecayTableInt[i] = (i * Env_Al) / Env_At;
    }
  } else {
    int i;
    float val = 1.0;
    for (i = 0; val <= Env_Al; i++) {
      val *= Env_ACoeff;
      attackDecayTableInt[i] = (int)val;
    }
    Env_At = i - 1;
    Env_Al = attackDecayTableInt[1 - i];
  }

  // Decay
  for (int i = Env_At; i < Env_Dt + Env_At; i++) {
    attackDecayTableInt[i] = Env_Al - ((i - Env_At) / (float)Env_Dt) * (Env_Al - Env_Sl);
  }

  // Release
  for (int i = 0; i < Env_Rt; i++) {
    releaseTableInt[i] = (Env_Sl - (i / (float)Env_Rt) * Env_Sl);
  }

  //Complete Release
  int newReleaseTableLenght = Env_Rt * 255 / Env_Sl;
  for (int i = 0; i < newReleaseTableLenght; i++) {
    completeReleaseTableInt[i] = (255 - (i / (float)newReleaseTableLenght) * 255);
  }

  //Reverse Complete Release
  for (int i = 0; i < 255; i++) {
    ReverseCompleteReleaseTableInt[i] = (255 - i) * newReleaseTableLenght / 255;
  }
  Env_R_startIndex = getReleaseIndex(Env_Sl);
}

int Envelope::getReleaseIndex(int val) {
  return ReverseCompleteReleaseTableInt[val];
}
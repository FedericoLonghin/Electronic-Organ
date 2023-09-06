

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
  short noteLife_ms = currentTime_ms - eventTime;
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
}
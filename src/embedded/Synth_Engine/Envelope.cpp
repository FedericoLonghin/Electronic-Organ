#include "Arduino.h"
#include "Envelope.h"

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
  for (int i = 0; i < Env_At; i++) {
    attackDecayTable[i] = (i * Env_Al) / Env_At;
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

  //Reverse Attack
  for (int i = 0; i < 256; i++) {
    ReverseAttackTable[i] = i * Env_At / Env_Al;
  }
  //Reverse Complete Release
  for (int i = 0; i < 256; i++) {
    ReverseCompleteReleaseTable[i] = (255 - i) * Env_R_completeTableLength / 255;
  }
}

int Envelope::getReleaseIndex(int val) {
  return ReverseCompleteReleaseTable[val];
}

int Envelope::getAttackIndex(int val) {
  return ReverseAttackTable[val];
}
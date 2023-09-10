#ifndef Envelope_h
#define Envelope_h
#include "Arduino.h"

#define Sample_Rate 40000
#define Envelope_AttackDecay_Table_Length 10000
#define Envelope_Release_Table_Length 5000
class Envelope {
public:
  int Env_At;
  int Env_Al;
  int Env_Dt;
  int Env_Sl;
  int Env_Rt;
  int Env_R_completeTableLength = 0;
  // private:
  byte attackDecayTable[Envelope_AttackDecay_Table_Length];
  byte completeReleaseTable[Envelope_Release_Table_Length];
  unsigned int ReverseAttackTable[256];
  unsigned int ReverseCompleteReleaseTable[256];

public:
  int getAmplitude(unsigned long noteLife, bool isKeyPressed, unsigned int releaseStartingPoint, bool *toBeDeleted);
  void reloadTable();
  int getReleaseIndex(int val);
  int getAttackIndex(int val);
};
#endif
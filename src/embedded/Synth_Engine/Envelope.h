#ifndef Envelope_h
#define Envelope_h
#include "Arduino.h"
#include "Definitions.h"
#include "AudioObject.h"



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
  int getAmplitudeNew(AudioObject *Obj);
  void reloadTable();
  int getReleaseIndex(int val);
  int getAttackIndex(int val);
};
#endif
#ifndef Sound_h
#define Sound_h
#include "Arduino.h"
#include "Definitions.h"
#include "Envelope.h"

class Sound {
public:
  Envelope ADSR;
  byte Wavetype;
  bool EFX_tremolo_enable = false;
  int EFX_tremolo_speed = 10;
  float EFX_tremolo_depth = 5;

  void setDefaultParam() {

    ADSR.Env_At = 100;
    ADSR.Env_Al = 255;
    ADSR.Env_Dt = 100;
    ADSR.Env_Sl = 200;
    ADSR.Env_Rt = 200;
    ADSR.reloadTable();
    Wavetype = 0;
  }
};


#endif
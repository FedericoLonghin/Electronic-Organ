#ifndef Sound_h
#define Sound_h
#include "Arduino.h"

class Sound {
public:
  Envelope ADSR;
  byte Wavetype;
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
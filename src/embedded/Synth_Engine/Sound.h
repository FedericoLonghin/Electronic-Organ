#ifndef Sound_h
#define Sound_h
#include "Arduino.h"
#include "Definitions.h"
#include "Envelope.h"


class LFO {
private:
  int freq_pv;
  bool freq_ramp_enable_next_step = false;
  unsigned int autoIncrementIndex = 0;
public:
  int freq_sp;
  float WaveTable[Wavetable_Length];
  LFO();
  void begin(int _freq);
  void generateTable();
  float getValAutoIncrement(bool _increment);
  void setSpeed(int _speed);
};

class Tremolo {
private:
  int speed_sp;    //[1,Inf[
  float depth_sp;  //[0,10]
  float depth_pv;
  LFO _tremLFO;

public:
  bool enable = false;
  void begin(int _speed, int _depth);
  void setSpeed(int _speed);
  void setDepth(float _depth);
  float getVal(bool _increment);
};

class Sound {
public:
  Envelope ADSR;
  byte Wavetype;

  Tremolo Trem;
  void setDefaultParam() {
    Trem.begin(1, 5);
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
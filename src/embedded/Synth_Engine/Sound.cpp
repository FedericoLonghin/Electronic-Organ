#include "Arduino.h"
#include "Sound.h"

/*
 *  LFO
 */

LFO::LFO() {}

void LFO::begin(int _freq) {
  this->freq_sp = _freq;
  this->freq_pv = _freq;
  generateTable();
}

void LFO::generateTable() {
  for (int i = 0; i < Wavetable_Length; i++) {
    WaveTable[i] = (sin(i * TWO_PI / (float)Wavetable_Length) + 1) / 2;
  }
}

float LFO::getValAutoIncrement(bool _increment) {
  byte addr = (byte)(((autoIncrementIndex * freq_pv) % Sample_Rate) * (float)(Wavetable_Length / (float)Sample_Rate));

  if (addr == 0 && freq_ramp_enable_next_step) {
    if (freq_pv < freq_sp) {
      freq_pv += 1;
      Serial.println("freq_pv++");
      autoIncrementIndex = 0;
    } else if (freq_pv > freq_sp) {
      freq_pv -= 1;
      Serial.println("freq_pv--");
      autoIncrementIndex = 0;
    }
    freq_ramp_enable_next_step = false;
  }

  if (addr != 0) { freq_ramp_enable_next_step = true; }

  if (_increment) { autoIncrementIndex++; }
  if (autoIncrementIndex >= Sample_Rate) autoIncrementIndex = 0;
  return WaveTable[addr];
}

void LFO::setSpeed(int _speed) {
  freq_sp = _speed;
}

/*
 *  Tremolo
 */

void Tremolo::begin(int _speed, int _depth) {
  this->speed_sp = _speed;
  this->depth_sp = _depth;
  this->depth_pv = _depth;
  this->_tremLFO.begin(_speed);
}

void Tremolo::setSpeed(int _speed) {
  _tremLFO.freq_sp = _speed;
}

void Tremolo::setDepth(float _depth) {
  depth_sp = _depth;
}

float Tremolo::getVal(bool _increment) {
  if (!enable) return 1;
  if (depth_pv < depth_sp) depth_pv += 0.001;
  else if (depth_pv > depth_sp) depth_pv -= 0.001;

  return 1 + (_tremLFO.getValAutoIncrement(_increment) * (depth_pv / (float)10));
}
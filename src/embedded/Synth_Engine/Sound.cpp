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
  for (int i = 0; i < LFO_Wavetable_Length; i++) {
    WaveTable[i] = (sin(i * TWO_PI / (float)LFO_Wavetable_Length) + 1) / 2;
  }
}

float LFO::getValAutoIncrement(bool _increment) {
  byte addr = (byte)(((autoIncrementIndex * freq_pv) % Sample_Rate) * (float)(LFO_Wavetable_Length / (float)Sample_Rate));

  if (addr == 0 && freq_ramp_enable_next_step) {
    if (freq_pv < freq_sp) {
      freq_pv += 1;
      autoIncrementIndex = 0;
    } else if (freq_pv > freq_sp) {
      freq_pv -= 1;
      autoIncrementIndex = 0;
    }
    freq_ramp_enable_next_step = false;
  }

  if (addr != 0) { freq_ramp_enable_next_step = true; }

  if (_increment) { autoIncrementIndex++; }
  if (autoIncrementIndex >= Sample_Rate) autoIncrementIndex = 0;
  return WaveTable[addr];
}

float LFO::getValAutoIncrementNew(bool _increment) {
  byte addr = (byte)(((autoIncrementIndex * freq_pv) % Sample_Rate) * (float)(LFO_Wavetable_Length / (float)Sample_Rate));

  if (addr == 0) {
    if (freq_ramp_enable_next_step) {

      if (freq_pv < freq_sp) {
        freq_pv += 1;
        autoIncrementIndex = 0;
      } else if (freq_pv > freq_sp) {
        freq_pv -= 1;
        autoIncrementIndex = 0;
      }
      freq_ramp_enable_next_step = false;
    }
  } else {
    freq_ramp_enable_next_step = true;
  }

  if (_increment) { autoIncrementIndex++; }
  if (autoIncrementIndex >= Sample_Rate) autoIncrementIndex = 0;
  return WaveTable[addr];
}

void LFO::setSpeed(int _speed) {
  freq_sp = _speed;
}

void LFO::increment() {
  autoIncrementIndex++;
  if (autoIncrementIndex >= Sample_Rate) autoIncrementIndex = 0;
}

/*
 *  Tremolo
 */

void Tremolo::begin(int _speed, int _depth) {
  this->speed_sp = _speed;
  this->depth_sp = _depth;
  this->depth_pv = _depth;
  // this->enable = true;
  this->_tremLFO.begin(_speed);
}

void Tremolo::setSpeed(int _speed) {
  if (_speed <= 0) _speed = 1;
  _tremLFO.freq_sp = _speed;
}

void Tremolo::setDepth(float _depth) {
  if (_depth > 10) _depth = 10;
  if (_depth < 0) _depth = 0;
  depth_sp = _depth;
}

float Tremolo::getVal(bool _increment) {
  if (!enable) return 1;
  if (depth_pv < depth_sp) depth_pv += 0.001;
  else if (depth_pv > depth_sp) depth_pv -= 0.001;
  if (depth_sp == 0.0 & depth_pv <= 0.001) enable = false;
  return 1 + (_tremLFO.getValAutoIncrement(_increment) * (depth_pv / (float)10));
}

float Tremolo::getValNew(bool _increment) {
  if (!enable) return 1;
  if (depth_pv < depth_sp) depth_pv += 0.001;
  else if (depth_pv > depth_sp) depth_pv -= 0.001;
  if (depth_sp == 0.0 & depth_pv <= 0.001) enable = false;
  return 1 + (_tremLFO.getValAutoIncrementNew(_increment) * (depth_pv / (float)10));
}
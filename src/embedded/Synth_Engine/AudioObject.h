#ifndef AudioObject_h
#define AudioObject_h
#include "Arduino.h"

const float NoteFrequency[12] = { 4186.01, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040, 7458.62, 7902.13 };

class AudioObject {
public:
  int frequency;
  int realNote;
  // int channel;
  unsigned long ticksFromLastEvent;
  unsigned int releaseStartingPoint;
  bool isKeyPressed;
  bool toBeDeleted;
  int sound;

  AudioObject(int note, unsigned long ticks, int _sound) {
    this->frequency = NoteFrequency[note % 12] / pow(2, (8 - (note / 12)));
    this->realNote = note;
    this->isKeyPressed = true;
    this->ticksFromLastEvent = ticks;
    this->toBeDeleted = false;
    this->sound = _sound;
    Serial.printf("Creating note with %d ticks\n", ticksFromLastEvent);
  }
};

#endif
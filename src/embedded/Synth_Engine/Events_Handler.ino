

#define MAX_AUDIO_OBJECT_NUMBER 10

AudioObject *AudioObjectList[MAX_AUDIO_OBJECT_NUMBER];

int currentAudioObjectsNumber = 0;

//Add an AudioObject to AudioObjectList
int noteOn(int freq) {
  if (currentAudioObjectsNumber >= MAX_AUDIO_OBJECT_NUMBER) {
    return 0;
  }
  AudioObjectList[currentAudioObjectsNumber] = new AudioObject(freq, 10);
  currentAudioObjectsNumber++;
  return 1;
}

//Remove an AudioObject to AudioObjectList
void noteOff(int Frequency) {
}
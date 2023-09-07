
class AudioObject {
public:
  int volume;
  int frequency;
  int id;
  unsigned long eventTime;
  unsigned long ticksFromLastEvent;
  unsigned int releaseStartingPoint;
  bool isKeyPressed = false;
  bool justReleased = false;

  AudioObject(int id, int vol, unsigned long stime) {

    this->volume = vol;
    this->frequency = NoteFrequency[id % 12] * pow(2, id / 12);
    this->id = id;
    this->eventTime = stime;
    this->isKeyPressed = true;
    this->ticksFromLastEvent = 0;
    // Serial.printf("Created Obj with time:%d\t current:%d\n", this->eventTime,currentTime_ms);
  }
};

AudioObjectListMenager::AudioObjectListMenager() {

  _currentlyPlayingNote = 0;
}

bool AudioObjectListMenager::start(int id) {

  if (id <= 0) return 0;
  int locat = find(id);
  if (locat != -1) {  //this note is already present
    AudioObjectList[locat] = new AudioObject(id, 10, currentTime_ms);
  } else {

    if (_currentlyPlayingNote >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[_currentlyPlayingNote] = new AudioObject(id, 10, currentTime_ms);

    _currentlyPlayingNote++;
  }
  fastNoteArray[find(id)] = AudioObjectList[find(id)]->frequency;
  return true;
}

bool AudioObjectListMenager::release(int id) {
  int locat = find(id);
  if (locat == -1) return false;
  AudioObjectList[locat]->releaseStartingPoint = env.getReleaseIndex(env.getNewAmplitudeInt(AudioObjectList[locat]->ticksFromLastEvent, true, 0));
  // Serial.printf("EventTime:%d\tnote Ampl: %d\tindex:%d\trecalc:%d\n",AudioObjectList[locat]->ticksFromLastEvent, env.getNewAmplitudeInt(AudioObjectList[locat]->ticksFromLastEvent, true, 0),AudioObjectList[locat]->releaseStartingPoint,env.getNewAmplitudeInt(0, false, AudioObjectList[locat]->releaseStartingPoint));
  AudioObjectList[locat]->isKeyPressed = false;
  AudioObjectList[locat]->eventTime = currentTime_ms;
  AudioObjectList[locat]->ticksFromLastEvent = 0;

  return true;
}

bool AudioObjectListMenager::stop(int id) {
  int locat = find(id);
  if (locat == -1) return false;
  if (_currentlyPlayingNote > 1) {
    AudioObjectList[locat] = AudioObjectList[_currentlyPlayingNote - 1];
    fastNoteArray[locat] = fastNoteArray[_currentlyPlayingNote - 1];
  }
  _currentlyPlayingNote--;
  return true;
}


void AudioObjectListMenager::cleanSilentObjects() {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    // Serial.print("aa");
    // Serial.println(i);
    if (!AudioObjectList[i]->isKeyPressed)
      if (currentTime_ms > AudioObjectList[i]->eventTime + env.Env_Rt)
        stop(AudioObjectList[i]->id);
    return;
  }
}

int AudioObjectListMenager::find(int id) {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    if (AudioObjectList[i]->id == id) return i;
  }
  return -1;
}


int AudioObjectListMenager::getActiveNotesNumber() {
  return _currentlyPlayingNote;
}

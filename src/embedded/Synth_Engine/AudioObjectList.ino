
class AudioObject {
public:
  int frequency;
  int id;
  unsigned long ticksFromLastEvent;
  unsigned int releaseStartingPoint;
  bool isKeyPressed;
  bool toBeDeleted;

  AudioObject(int id) {
    this->frequency = NoteFrequency[id % 12] * pow(2, id / 12);
    this->id = id;
    this->isKeyPressed = true;
    this->ticksFromLastEvent = 0;
    this->toBeDeleted = false;
  }
};

AudioObjectListMenager::AudioObjectListMenager() {

  _currentlyPlayingNote = 0;
}

bool AudioObjectListMenager::start(int id) {

  if (id <= 0) return 0;
  int locat = find(id);
  if (locat != -1) {  //this note is already present
    AudioObjectList[locat] = new AudioObject(id);
  } else {

    if (_currentlyPlayingNote >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[_currentlyPlayingNote] = new AudioObject(id);

    _currentlyPlayingNote++;
  }
  fastNoteArray[find(id)] = AudioObjectList[find(id)]->frequency;
  return true;
}

bool AudioObjectListMenager::release(int id) {
  int locat = find(id);
  if (locat == -1) return false;
  AudioObjectList[locat]->releaseStartingPoint = env.getReleaseIndex(env.getAmplitude(AudioObjectList[locat]->ticksFromLastEvent, true, 0, 0));
  AudioObjectList[locat]->isKeyPressed = false;
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

    if (AudioObjectList[i]->toBeDeleted) {
      stop(AudioObjectList[i]->id);
    }
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

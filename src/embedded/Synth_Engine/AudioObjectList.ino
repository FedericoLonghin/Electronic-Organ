
class AudioObject {
public:
  int volume;
  int frequency;
  int id;
  int pos_in_fast_note_array;

  AudioObject(int id, float freq, int vol) {
    this->volume = vol;
    this->frequency = freq;
    this->id = id;
  }
};

AudioObjectListMenager::AudioObjectListMenager() {

  _currentlyPlayingNote = 0;
}

bool AudioObjectListMenager::add(int id) {
  if (id <= 0) return 0;
  int locat = find(id);
  Serial.printf("find(id): %d\n", find(id));
  if (locat != -1) {  //this note is already present
    AudioObjectList[locat] = new AudioObject(id, (NoteFrequency[id % 12] * pow(2, id / 12)), 10);
  } else {

    if (_currentlyPlayingNote >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[_currentlyPlayingNote] = new AudioObject(id, (int)(NoteFrequency[id % 12] * pow(2, id / 12)), 10);

    _currentlyPlayingNote++;
  }
  Serial.printf("find(id)2: %d, %d\n", find(id), AudioObjectList[find(id)]->frequency);
  fastNoteArray[find(id)] = AudioObjectList[find(id)]->frequency;
  return true;
}

bool AudioObjectListMenager::remove(int id) {
  int locat = find(id);
  if (locat == -1) return false;
  if (_currentlyPlayingNote > 1) {
    AudioObjectList[locat] = AudioObjectList[_currentlyPlayingNote - 1];
    fastNoteArray[locat]=fastNoteArray[_currentlyPlayingNote-1];
  }
  _currentlyPlayingNote--;
  return true;
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

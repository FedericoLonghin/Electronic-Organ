/*
 * Every AudioObject has:
 * - Note
 * - Channel
 * - id (calculated with Note and Channel)
 * 
 * activeNoteList[] contains all active AudioObject's ids
 */

//Id type
typedef int AO_Id;


class AudioObject {
public:
  int frequency;
  int note;
  int channel;
  unsigned long ticksFromLastEvent;
  unsigned int releaseStartingPoint;
  bool isKeyPressed;
  bool toBeDeleted;

  AudioObject(int note, unsigned long ticks) {
    this->frequency = NoteFrequency[note % 12] / pow(2, (8 - (note / 12)));
    this->note = note;
    this->isKeyPressed = true;
    this->ticksFromLastEvent = ticks;
    this->toBeDeleted = false;
    Serial.printf("Creating note with %d ticks\n", ticksFromLastEvent);
  }
};

AudioObjectListMenager::AudioObjectListMenager() {

  _currentlyPlayingNote = 0;
}

bool AudioObjectListMenager::start(int note, int channel) {
  if (note <= 0) return 0;
  int locat = find_inActiveNoteList(getNoteId(note, channel));
  if (locat != -1) {  //this note is already present
    Serial.println("New note is already present");
    byte ampl = env.getAmplitude(AudioObjectList[getNoteId(note, channel)]->ticksFromLastEvent, 0, AudioObjectList[getNoteId(note, channel)]->releaseStartingPoint, &AudioObjectList[getNoteId(note, channel)]->toBeDeleted);
    int envelopeIndexStart = env.getAttackIndex(ampl);
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note, envelopeIndexStart * (Sample_Rate / 1000));
    Serial.printf("Ampl:%d\tStart:%d\n", ampl, envelopeIndexStart);
  } else {
    if (_currentlyPlayingNote >= _MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    AudioObjectList[getNoteId(note, channel)] = new AudioObject(note, 0L);
    activeNoteList[_currentlyPlayingNote] = getNoteId(note, channel);
    _currentlyPlayingNote++;
  }
  return true;
}


bool AudioObjectListMenager::release(int note, int channel) {
  int id = getNoteId(note, channel);
  AudioObjectList[id]->releaseStartingPoint = env.getReleaseIndex(env.getAmplitude(AudioObjectList[id]->ticksFromLastEvent, true, 0, 0));
  AudioObjectList[id]->isKeyPressed = false;
  AudioObjectList[id]->ticksFromLastEvent = 0;

  return true;
}

//remove stopped Object's id from activeNoteList
bool AudioObjectListMenager::stop(int id) {
  Serial.printf("Deleting note:%d\n", id);
  int locat = find_inActiveNoteList(id);
  if (locat == -1) return false;
  if (_currentlyPlayingNote > 1) {
    activeNoteList[locat] = activeNoteList[_currentlyPlayingNote - 1];
  }
  _currentlyPlayingNote--;
  return true;
}

void AudioObjectListMenager::cleanSilentObjects() {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    if (AudioObjectList[activeNoteList[i]]->toBeDeleted) {
      stop(activeNoteList[i]);
    }
  }
}

// return Position in activeNoteList[]
int AudioObjectListMenager::find_inActiveNoteList(int id) {
  for (int i = 0; i < _currentlyPlayingNote; i++) {
    if (activeNoteList[i] == id) return i;
  }
  return -1;
}

int AudioObjectListMenager::getActiveNotesNumber() {
  return _currentlyPlayingNote;
}
void AudioObjectListMenager::stopAll() {
  _currentlyPlayingNote = 0;
}

int getNoteId(int note, int channel) {
  return channel * MAX_AUDIO_OBJECT_PER_CHANNEL + note;
}
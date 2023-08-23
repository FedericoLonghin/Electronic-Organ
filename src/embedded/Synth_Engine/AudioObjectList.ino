const float NoteFrequency[12] = { 16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87 };


class AudioObject {
public:
  int volume;
  int frequency;
  int id;

public:

  AudioObject(int id, int freq, int vol) {
    Serial.printf("Created note. Id: %d \n",id);
    this->volume = vol;
    this->frequency = freq;
    this->id = id;
  }
};



AudioObjectListNew::AudioObjectListNew(int num) {

  AudioObjectListNew::currentlyPlayingNote = 0;
}

bool AudioObjectListNew::add(int id) {
  int locat = find(id);

  if (locat != -1) {  //this note is already present
    NEWAudioObjectList[locat] = new AudioObject(id, (int)(NoteFrequency[id % 12] * pow(2, id / 12)), 10);
  } else {

    if (currentlyPlayingNote >= MAX_AUDIO_OBJECT_NUMBER) {
      return 0;
    }
    NEWAudioObjectList[currentlyPlayingNote] = new AudioObject(id, (int)(NoteFrequency[id % 12] * pow(2, id / 12)), 10);

    currentlyPlayingNote++;
  }
  return true;
}

bool AudioObjectListNew::remove(int id) {
  int locat = find(id);
  if (locat == -1) return false;
  if (currentlyPlayingNote > 1) {
    NEWAudioObjectList[locat] = NEWAudioObjectList[currentlyPlayingNote - 1];
  }
  currentlyPlayingNote--;
  return true;
}

int AudioObjectListNew::find(int id) {
  for (int i = 0; i < currentlyPlayingNote; i++) {
    Serial.println(NEWAudioObjectList[i]->id);
    if (NEWAudioObjectList[i]->id == id) return i;
  }
  return -1;
}

int AudioObjectListNew::getFrequency(int id) {
  return NEWAudioObjectList[id]->frequency;
}

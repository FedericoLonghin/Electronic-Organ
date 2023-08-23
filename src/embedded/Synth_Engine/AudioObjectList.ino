class AudioObject {
public:
  int volume;
  int frequency;
  int id;

public:
  // AudioObject(int freq) {
  //   volume = 100;
  //   frequency = freq;
  // }
  AudioObject(int freq, int vol) {
    volume = vol;
    frequency = freq;
    id = freq;
  }
};

class AudioObjectListNew {
  
public:
  int length;

  bool add() {
    return false;
  }
  bool remove() {
    return false;
  }
  int find() {
    return false;
  }
};
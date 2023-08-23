class AudioObject {
private:
  int volume;
  int frequency;

public:
  // AudioObject(int freq) {
  //   volume = 100;
  //   frequency = freq;
  // }
  AudioObject(int freq, int vol) {
    volume = vol;
    frequency = freq;
  }
};

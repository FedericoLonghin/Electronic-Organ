float Envelope::getAmplitude(unsigned long startTime) {
  int noteLen = millis() - startTime;
  if (noteLen < Env_A_t) {
    return noteLen / (float)Env_A_t;
  }
 
  
  return 0;
}
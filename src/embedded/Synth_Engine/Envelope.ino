

float Envelope::getAmplitude(unsigned long startTime) {
  int noteLen = millis() - startTime;
  if (noteLen < Env_A_t) {
    return noteLen *(Env_A_l / (float)Env_A_t);
  }
  if (noteLen < Env_A_t+Env_D_t) {
    return Env_A_l+  ((noteLen-Env_A_t)/(float)Env_D_t)*(Env_S_l -Env_A_l);
  }


  return Env_S_l;
}
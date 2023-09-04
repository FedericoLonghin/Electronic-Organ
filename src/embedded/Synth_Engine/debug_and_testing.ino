unsigned long int debug_counterStartTime;
unsigned long int debug_counterEndTime;
unsigned long int debug_counterIteration;
unsigned long int debug_counterMaxIteration;
bool debug_isCounterActive=false;

void startCounter(int max, bool count_first) {
  debug_counterIteration = count_first ? 1 : 0;
  debug_counterMaxIteration = max;
  debug_isCounterActive=true;
  debug_counterStartTime = millis();
}
void endCounter() {

  debug_counterEndTime = millis();
debug_isCounterActive=false;
  Serial.printf("Counter ended: %d ticks in %d millisecond\n",debug_counterIteration,debug_counterEndTime-debug_counterStartTime);
}

void tick() {
  if(debug_isCounterActive){

  debug_counterIteration++;
  if (debug_counterIteration >= debug_counterMaxIteration) endCounter();
  }
}

void printFloatArray(float arr[],int len){
    Serial.println("Arr:");
  for (int i=0;i<len;i++){
    Serial.print(i);
    Serial.print("\t");
    Serial.println(arr[i]);
  }
    Serial.println("End.");
}
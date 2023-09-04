hw_timer_t *Timer0_Cfg = NULL;

void IRAM_ATTR Timer0_ISR();


//Interrupt setup code
//Create an Interrupt at 10KHz on Core 1
void setupTimerInterrupt() {
  Timer0_Cfg = timerBegin(0, 8, true);
  timerAttachInterrupt(Timer0_Cfg, *Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);
}

//Interrupt Service Routine
//Runs et 10KHz frequency
void IRAM_ATTR Timer0_ISR() {
  if (OutBufferIndex >= Sample_rate) OutBufferIndex = 0;
  dacWrite(25, wave[OutBufferIndex]);
  if(OutBufferIndex%10==0)myMillis++;
  OutBufferIndex++;
}
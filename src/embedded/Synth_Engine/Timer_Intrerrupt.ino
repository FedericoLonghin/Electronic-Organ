hw_timer_t *Timer0_Cfg = NULL;

void IRAM_ATTR Timer0_ISR();


//Interrupt setup code
//Create an Interrupt at 10KHz on Core 1
void setupTimerInterrupt() {
  Timer0_Cfg = timerBegin(0, 2, true);
  timerAttachInterrupt(Timer0_Cfg, *Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);
}

//Interrupt Service Routine
//Runs et 10KHz frequency
// bool stat = false;
// void IRAM_ATTR Timer0_ISR() {

//   stat = !stat;

//   dacWrite(25, stat);
//   // if (OutBufferIndex % 10 == 0) {
//   //   currentTime_ms++;
//   // }


// }
void IRAM_ATTR Timer0_ISR() {
  if (OutBufferIndex >= MAGIC_BUFFER_OFFSET * 2) {
    OutBufferIndex = 0;

    newSampleREQ = true;
    newSampleREQ_SectionToFill = true;
  } else if (OutBufferIndex == MAGIC_BUFFER_OFFSET) {
    newSampleREQ = true;
    newSampleREQ_SectionToFill = false;
  }

  dacWrite(25, wave[OutBufferIndex]);
  OutBufferIndex++;
}
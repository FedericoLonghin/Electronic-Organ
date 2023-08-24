#include "settings.h"
#include "MIDIUSB.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);




void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(CLK, OUTPUT);
  pinMode(LD, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(ANALOG_SELECT_0, OUTPUT);
  pinMode(ANALOG_SELECT_1, OUTPUT);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  // pinMode(13, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.println("Serial Keyboard Started! \n");
}

void loop() {
Serial.println("loop");

  //  printAnalogSliders();
  //printAnalogBuffer();
  // fetchAnalog();
  // checkAnalog();
  extractKeyboardStatus();
  // printRAWAalog();

  // delayMicroseconds(2000);
  getButtonPressed();
  // checkPedal();
  // checkEncoder();
  // printArray(keyStatus, BINARY_IN_LENGHT);
  //Serial.println();

  
  // if (showChords) {
  //   checkNewChord();
  // }

  // updateDisplay();
}
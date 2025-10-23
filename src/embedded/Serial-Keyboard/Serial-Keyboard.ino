#define SERIAL_DEBUG
#define LED_DEBUG
#define NEW_VERSION

#include "settings.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#ifdef LED_DEBUG
#define DEBUG_LED_PIN 13
#endif

void setup() {

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif
  Serial1.begin(9600);
  pinMode(CLK, OUTPUT);
  pinMode(LD, OUTPUT);
  pinMode(OUT, INPUT);
  pinMode(ANALOG_SELECT_0, OUTPUT);
  pinMode(ANALOG_SELECT_1, OUTPUT);
  // pinMode(13, INPUT_PULLUP);
  // pinMode(12, INPUT_PULLUP);
  // pinMode(11, INPUT_PULLUP);
#ifdef LED_DEBUG
  pinMode(DEBUG_LED_PIN, OUTPUT);
#endif

  pinMode(ANALOG_SELECT_0, OUTPUT);
  pinMode(ANALOG_SELECT_1, OUTPUT);
  digitalWrite(ANALOG_SELECT_0, 0);
  digitalWrite(ANALOG_SELECT_1, 0);

  lcd.init();
  lcd.backlight();


#ifdef SERIAL_DEBUG
  Serial.println("Serial Keyboard Started! \n");
#endif
  SendParam_float(244, 0, 3, 0, 1.234f);
}

void loop() {
  SendParam_float(244, 0, 3, 0, 1.234f);

  delay(1000);
  // int msg_len_i = 0;
  // bool payload = false;
  // String msg = "";
  // if (Serial1.available()) {
  //   msg = Serial1.readStringUntil('.');
  //   payload = true;
  //   msg_len_i++;
  //   // delay(5);
  // }
  // // Serial.println("a");
  // if (payload) {

  //   if (msg.startsWith("reloadSt")) {
  //     Serial.println("reload Stop REQ");
  //     sendAllStopsStatus();
  //   }
  //   payload = false;
  // }


  // Serial.println("loop");

  //  printAnalogSliders();
  //printAnalogBuffer();
  //  fetchAnalog();
  // checkAnalog();
  // extractKeyboardStatus();
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
  // Serial.println("Analog Val:");
  // for(int i=0;i<16;i++){
  // //   Serial.print(i);
  //   Serial.print("\t ");
  //   Serial.print(istantAnalogValue(i));
  // }
  // Serial.print("A:0,B:1023,C:");
  // Serial.print(analogRead(A2));
  // Serial.print(",D:");
  // Serial.println(analogRead(A3));
}
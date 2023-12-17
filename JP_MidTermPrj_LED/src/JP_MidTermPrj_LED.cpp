/*
 * Project JP_MidTermPrj_LEDtri
 * Description:LED test 
 * Author:JPFunk
 * Date:12/16/2023
 */
#include "Particle.h"

const int REDLEDPIN = D13;
const int GREENLEDPIN = D14;
const int BLUELEDPIN = D15;
const int LEDDELAY = 20;
int j;
int motionPin=D10;
int pirState = LOW; 
int val = 0; 
SYSTEM_MODE(SEMI_AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
pinMode(motionPin, INPUT);
pinMode(REDLEDPIN, OUTPUT);
pinMode(GREENLEDPIN, OUTPUT);
pinMode(BLUELEDPIN, OUTPUT);
j = 100;

//pinMode(motionPin, INPUT_PULLUP);
Serial.begin(9600);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
 val=digitalRead(motionPin);
 if (val==HIGH){
  analogWrite(REDLEDPIN, HIGH);
 if (pirState==LOW){
   Serial.printf("Motion detected!\n");
  pirState = HIGH;
  }
 } else{
  analogWrite(REDLEDPIN, LOW);
  if (pirState==HIGH){
 Serial.printf("Motion ended!\n");
 pirState = LOW;
 }
 }

//   int motionState = digitalRead(motionPin);
// if(motionState == LOW) {
//   analogWrite (REDLEDPIN, HIGH);
//   } else {
//   analogWrite (REDLEDPIN, LOW);
//   }
  }
  
  // for (j=0; j <= 255; j++) {
  //   analogWrite (REDLEDPIN, j);
  //   delay(20);
  //  }
  // for (j=255; j >= 0; j--) {
  //   analogWrite (REDLEDPIN, j);
  //   delay(20);
  //  }
  
  //  for (j=0; j <= 255; j++) {
  //   analogWrite (GREENLEDPIN, j);
  //   delay(20); 
  //  }
  // for (j=255; j >= 0; j--) {
  //   analogWrite (GREENLEDPIN, j);
  //   delay(20);
  //  }

  //   for (j=0; j <= 255; j++) {
  //   analogWrite (BLUELEDPIN, j);
  //   delay(20); 
  //  }
  // for (j=255; j >= 0; j--) {
  //   analogWrite (BLUELEDPIN, j);
  //   delay(20);
  //  }
// }
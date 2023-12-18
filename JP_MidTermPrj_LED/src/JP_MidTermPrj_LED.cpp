/*
 * Project JP_MidTermPrj_LEDtri
 * Description:LED test 
 * Author:JPFunk
 * Date:12/16/2023
 */
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
// OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);

const int REDLEDPIN = D13;
const int GREENLEDPIN = D14;
const int BLUELEDPIN = D15;
const int LASERPIN = D19;
const int LEDDELAY = 20;
int j;
int motionPin=D11;
int pirState = LOW; 
int moval = 0; 
SYSTEM_MODE(SEMI_AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
pinMode(motionPin, INPUT);
pinMode(REDLEDPIN, OUTPUT);
pinMode(GREENLEDPIN, OUTPUT);
pinMode(BLUELEDPIN, OUTPUT);
pinMode(LASERPIN, OUTPUT);
j = 100;

//pinMode(motionPin, INPUT_PULLUP);
Serial.begin(9600);
waitFor(Serial.isConnected,10000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(2000);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
 moval=digitalRead(motionPin);
 if (moval==HIGH){
  analogWrite(REDLEDPIN, HIGH);
  analogWrite(GREENLEDPIN, HIGH);
  analogWrite(BLUELEDPIN, HIGH);
  analogWrite(LASERPIN, HIGH);
  delay (10);

 if (pirState==LOW){
display.clearDisplay();
display.setCursor(0,0);
display.printf("Motion detected!\n");
display.display();
delay(2000);
  pirState = HIGH;
  }
 } else{
  analogWrite(REDLEDPIN, LOW);
  analogWrite(GREENLEDPIN, LOW);
  analogWrite(BLUELEDPIN, LOW);
  analogWrite(LASERPIN, LOW);
  if (pirState==HIGH){
display.clearDisplay();
display.setCursor(0,0);
display.printf("Motion ended!\n");
display.display();
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
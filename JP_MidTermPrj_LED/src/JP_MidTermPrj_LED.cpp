/*
 * Project JP_MidTermPrj_LEDtri
 * Description:LED test 
 * Author:JPFunk
 * Date:12/16/2023
 */
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <math.h>
// OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);

const int REDLEDPIN = D13;
const int GREENLEDPIN = D14;
const int BLUELEDPIN = D15;
const int LASERPIN = D19;
const int LEDDELAY = 20;
int j;
const int MYPIR =0;
int motionPin=D11;
int pirState = LOW; 
int moval = 0;

Button blackButton (D4);
bool pirOnOff;
// LED Sine Wave
float value , n ;
float t;
float y;
float s;
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
n = 0;

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
// pirOnOff = digitalRead(motionPin);
// display.clearDisplay();
// display.setCursor(0,0);
//Motion Sonsor PIR

moval=digitalRead(motionPin);
if (moval==HIGH){

//s=(0,1);
//y = 128 * sin(2 * M_PI * s * t) + 128;
// t = millis() / 3000.0;
// y = 128 * sin(2 * M_PI * 1/2.0 * t) + 128;
// analogWrite (REDLEDPIN, y);
// y = 128 * sin(2 * M_PI * 1/5.0 * t) + 128;
// analogWrite(GREENLEDPIN, y);
// y = 128 * sin(2 * M_PI * 1/7.0 * t) + 128;
// analogWrite(BLUELEDPIN, y);
analogWrite(REDLEDPIN, HIGH);
analogWrite(GREENLEDPIN, HIGH);
analogWrite(BLUELEDPIN, HIGH);
delay (2000);

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
pirOnOff = digitalRead(motionPin);
if (blackButton.isClicked()) {
display.clearDisplay();
display.setCursor(0,0);
display.printf("Turning on PIR!\n");
display.display();
pirOnOff= !pirOnOff;
}

if (blackButton.isClicked()) {
display.clearDisplay();
display.setCursor(0,0);
display.printf("Turning off PIR!\n");
display.display();
pirOnOff= !pirOnOff;
}
//   int motionState = digitalRead(motionPin);
// if(motionState == LOW) {
//   analogWrite (REDLEDPIN, HIGH);
//   } else {
//   analogWrite (REDLEDPIN, LOW);
//   }
  }
 //--------------------------------------------------------------------------------------------------------------- 
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
// Motion Sensor Functions for WEMO------------------------------------
moval=digitalRead(motionPin);
if (moval==HIGH){
wemoWrite (MYWEMO, HIGH);

if (pirState==LOW){
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("WEMO\n ON!\n");
display.display(); //added display code
delay(200);
pirState = HIGH;
}
} else{
wemoWrite(MYWEMO, LOW);

if (pirState==HIGH){
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("WEMO\nOFF!\n");
display.display(); //added display code
delay(200);
pirState = LOW;
}
}
/* 
 * Project JP_MidTermPrj
 * Author: JP
 * Date: 12/15/2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
// Include Particle Device OS APIs

#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Encoder.h"
//Hue
int BULB; 
int color;
int colorNum;
//Millis time for lights
long unsigned int lightTime;
int lightDelay;
// Button
Button grayButton (D3);
Button redButton (D2);
Button myEncBtn (D17);
bool hueOnOff;
bool wemoOnOff;
//Encoder
const int maxPos = 95;
const int minPos = 0;
int hueBright;
int hueColor;
int prevenc;
Encoder myEnc(D8,D9);
//Button myEncBtn(PIN);
// Encoder Button code
const int GREENBUTTONPIN=D17;
const int REDBUTTONPIN=D18;
int greenState,redState;
bool buttonState;
// Wemo
const int MYWEMO=0;
//Super Bright LED
const int REDLEDPIN=D13;
const int GREENLEDPIN=D14;
const int BLUELEDPIN=D15;
const int LEDDELAY=20;
int j;
//MotionSensor
int motionPin=D10;
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(MANUAL);
// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
// SuperBright LED
pinMode(REDLEDPIN, OUTPUT);
pinMode(GREENLEDPIN, OUTPUT);
pinMode(BLUELEDPIN, OUTPUT);
j = 100;
pinMode(motionPin, INPUT_PULLUP);

lightDelay=2000;
Serial.begin(9600);
waitFor(Serial.isConnected,15000);
BULB =1;
//Encoder
pinMode(GREENBUTTONPIN, OUTPUT);
pinMode(REDBUTTONPIN, OUTPUT);

WiFi.on();
WiFi.clearCredentials(); // prevent from connecting to DDCIOT
WiFi.setCredentials("IoTNetwork");
// If network requires a password
// setCredentials ( const "" NetworkName ", " Password ");
WiFi.connect();
while(WiFi.connecting()) {
Serial.printf(".");
}
Serial.printf("\n\n");
lightTime=millis();
} 

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
int motionState = digitalRead(motionPin);
if(motionState == LOW) {
  analogWrite (REDLEDPIN, HIGH);
  } else {
  analogWrite (REDLEDPIN, LOW);
  }

//SuperBright LED
  for (j=0; j <= 255; j++) {
    analogWrite (REDLEDPIN, j);
    delay(LEDDELAY); 
   }
  for (j=255; j >= 0; j--) {
    analogWrite (REDLEDPIN, j);
    delay(LEDDELAY);
   }

//Button Functions
if (grayButton.isClicked()) {
hueOnOff=!hueOnOff;
}
//Encoder Button to cycle through colors
if (myEncBtn.isPressed()) {
if (millis()-lightTime>lightDelay);
colorNum=HueRainbow[color%7];
color++;
lightTime=millis();
}

//Encoder Funtions
hueBright=myEnc.read();
if(hueBright<=0){
  hueBright=0;
  myEnc.write(0);
}
if(hueBright>=255){
  hueBright=255;
  myEnc.write(255);
}

//Encoder button
if (hueBright != prevenc){
Serial.printf("%i\n",hueBright);
prevenc = hueBright;
}
//Hue
Serial.printf("Setting color of bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
setHue(BULB,hueOnOff,colorNum,hueBright,255);
delay(100);

//Wemo button
Serial.printf("Turning on Wemo# %i\n",MYWEMO);
if (redButton.isClicked()) {
wemoOnOff= !wemoOnOff;
}
if (redButton.isClicked()) {
wemoOnOff= !wemoOnOff;
}
}


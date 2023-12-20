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
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
// OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);
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
Button blackButton (D4);
bool hueOnOff;
bool wemoOnOff;
bool pirOnOff;
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
const int LASERPIN = D19;
const int LEDDELAY=20;
int j;
//MotionSensor
const int MYPIR=0;
int motionPin=D11;
int pirState=LOW; 
int moval=0;
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
// Run the application and system concurrently in separate threads
//SYSTEM_THREAD(ENABLED);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
Serial.begin(9600);
waitFor(Serial.isConnected,10000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(2000);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
// SuperBright LED
pinMode(REDLEDPIN, OUTPUT);
pinMode(GREENLEDPIN, OUTPUT);
pinMode(BLUELEDPIN, OUTPUT);
pinMode(LASERPIN, OUTPUT);
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
//Motion PIR sensor
display.clearDisplay();
display.setCursor(0,0);
display.printf("Turning on PIR# %i\n",MYPIR);
display.display();
delay(2000);
if (blackButton.isClicked()) {
pirOnOff= !pirOnOff;
}
if (redButton.isClicked()) {
pirOnOff= !pirOnOff;
}

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
delay(2000);
pirState = LOW;
}
}

//SuperBright LED
  // for (j=0; j <= 255; j++) {
  //   analogWrite (REDLEDPIN, j);
  //   delay(LEDDELAY); 
  //  }
  // for (j=255; j >= 0; j--) {
  //   analogWrite (REDLEDPIN, j);
  //   delay(LEDDELAY);
  //  }

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
display.clearDisplay();
display.setCursor(0,0);
display.printf("Setting color of bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
display.display();
delay(2000);
setHue(BULB,hueOnOff,colorNum,hueBright,255);
delay(100);

//Wemo button
display.clearDisplay();
display.setCursor(0,0);
display.printf("Turning on Wemo# %i\n",MYWEMO);
display.display();
delay(2000);
if (redButton.isClicked()) {
wemoOnOff= !wemoOnOff;
}
if (redButton.isClicked()) {
wemoOnOff= !wemoOnOff;
}
}


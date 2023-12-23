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
#include <math.h>
#include "JPBitmap.h"
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
//const int LASERPIN = D19; //Temporary PIN for Laser
const int LEDDELAY=20;
int j;
//MotionSensor
const int MYPIR=0;
int motionPin=D11;
int pirState=LOW; 
int moval=0;
// LED Sine Wave
float value , n ;
float t;
float y;
float s;
// Millis
int startime;
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
//Motion Sensor PIR
pinMode(motionPin, INPUT);
// SuperBright LED
pinMode(REDLEDPIN, OUTPUT);
pinMode(GREENLEDPIN, OUTPUT);
pinMode(BLUELEDPIN, OUTPUT);
//pinMode(LASERPIN, OUTPUT); //Temporary Pin for Laser
j = 100;
n = 0;
//OLED Display functions
Serial.begin(9600);
waitFor(Serial.isConnected,10000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(1000);
// JP <3 IOT bitmap display
display.clearDisplay();
display.drawBitmap (0, 0, jpBitmap, 128, 64, WHITE);
display.display();
delay(2000);
// Invert Display
display.invertDisplay(true);
delay(1000);
display.invertDisplay(false);
delay(1000);
//Display Text
display.setTextSize(2);
display.setTextColor(WHITE);

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
lightTime= millis();
//startime = millis(); //Millis time for PIR
} 
// loop() runs over and over again, as quickly as it can execute.
void loop() {
// The core of your code will likely live here.
//Motion Sensor PIR----------------------------------------------------------------
moval=digitalRead(motionPin);
if (moval==HIGH){

t = millis() / 3000.0;
y = 128 * sin(2 * M_PI * 1/2.0 * t) + 128;
analogWrite (REDLEDPIN, y);
//analogWrite(REDLEDPIN, HIGH);
y = 128 * sin(2 * M_PI * 1/5.0 * t) + 128;
analogWrite(GREENLEDPIN, y);
//analogWrite(GREENLEDPIN, HIGH);
y = 128 * sin(2 * M_PI * 1/7.0 * t) + 128;
analogWrite(BLUELEDPIN, y);
//analogWrite(BLUELEDPIN, HIGH);
//millis(1000);
delay (10);

if (pirState==LOW){
display.clearDisplay();
display.setCursor(0,0);
display.printf("Motion\nDetected!\n");
display.display();
delay(200);
pirState = HIGH;
}
} else{
analogWrite(REDLEDPIN, LOW);
analogWrite(GREENLEDPIN, LOW);
analogWrite(BLUELEDPIN, LOW);

if (pirState==HIGH){
display.clearDisplay();
display.setCursor(0,0);
display.printf("Motion\nEnded!\n");
display.display();
delay(200);
pirState = LOW;
}
// PIR Button with added Display code-------------------------------------------------
pirOnOff = digitalRead(motionPin);
if (blackButton.isClicked()) {
display.clearDisplay();
display.setCursor(0,0);
display.printf("PIR On/Off\n");
display.display();
pirOnOff= !pirOnOff;
}
}
// Hue Button Functions with added Display code---------------------------------------
if (grayButton.isClicked()) {
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("Hue Light On/Off!\n");
display.display(); //added display code
hueOnOff=!hueOnOff;
}
//Encoder Button to cycle through colors with added Display code
if (myEncBtn.isPressed()) {
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("Set Bulb#%i\nColor %06i\n",BULB,HueRainbow[color%7]);
display.display(); //added display code
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
//Encoder button with added Display code
if (hueBright != prevenc){
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("Setting\nBrightness\nValue %i\n",hueBright);
display.display(); //added display code
prevenc = hueBright;
}
//delay(2000);
setHue(BULB,hueOnOff,colorNum,hueBright,255);
delay(100);
//-----------------------------------------------------------------------------------
//Wemo button with added Display code
//delay(2000);
if (redButton.isClicked()) {
display.clearDisplay(); //added display code
display.setCursor(0,0); //added display code
display.printf("Wemo#%i On/Off!\n",MYWEMO);
display.display(); //added display code
wemoOnOff= !wemoOnOff;
}
display.display(); //added display code
}
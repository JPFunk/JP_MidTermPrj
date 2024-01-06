/* 
 * Project JP_MidTermPrj
 * Author: JP
 * Date: 12/15/2023
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
// Include Particle Device OS APIs
// Header Files for JP MidTermPrj Pyramid-----------------------------
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Button.h"
#include "Colors.h"
#include "neopixel.h"
#include "Encoder.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "math.h"
//#include "JPBitmap.h" old custom BMP
#include "PyramidBitmap.h"
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

IoTTimer wemoTimer;
// Buttons
Button grayButton (D3);
Button redButton (D5);
Button myEncBtn (D17);
Button blackButton (D4);
//Mode Switch
const int modeSwitch= D6;

// Booleans for On Off
bool hueOnOff;
bool wemoOnOff;
bool neoOnOff;
bool movalOnOff;
bool modeOnOff;

// Special Button
int buttonPin;
int prevButtonState;
bool pullUp;
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
const int MYWEMO=3;
//NEOPIXEL
int ledState=LOW; 
const int PIXELCOUNT = 16; // Total number of NeoPixels might be 18 for the project!
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B); // declare object
const int neoPixBtn (D2);
int pixelAddr;
int colorCount;
int position;
int mySeq [PIXELCOUNT];
int x;
int temp;
int i;
int j;
int cycleCount;

//MotionSensor
const int MYPIR=0;
int motionPin=D11;
int pirState=LOW; 
int moval;
// RGB LED Sine Wave---------Not Using-------------------------------
// float value , n ;
// float t;
// float y;
// float s;

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
// NeoPixel Set Up----------------------------------------------------------------
pixel.begin ();
pixel.setBrightness (64); // bri is a value 0 - 255
pixel.clear ();
pixel.show (); // initialize all off
colorCount=0;
pixelAddr=0;
random (0,4); // LED assignment 0,1,2,3,4
rainbow[0,1,2,3,4];

// Mode Switch----------------------------
pinMode(modeSwitch, INPUT);
//Motion Sensor PIR
pinMode(motionPin, INPUT);

//NeoPixel--------------------------------
pixel.begin ();
pixel.setBrightness (10); // bri is a value 0 - 255
pixel.show (); // initialize all off

//OLED Display functions------------------
Serial.begin(9600);
waitFor(Serial.isConnected,15000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(1000);

// Pyramid bitmap display-------------------------------
display.clearDisplay();
//display.drawBitmap (0, 0, jpBitmap, 128, 64, WHITE);
display.drawBitmap (0, 0, pyramidBitmap, 128, 64, WHITE);
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

// Hue Light--------------------------------------------
lightDelay=2000;
Serial.begin(9600);
waitFor(Serial.isConnected,15000);
BULB =1;
//Encoder----------------------------------------------
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

// Switch Code OnOff functions for Wemo RedButton and Motion Sensor--------------
modeOnOff = digitalRead(modeSwitch);
moval=digitalRead(motionPin);
if (modeOnOff) {
//if (moval==HIGH){
  Serial.printf("%i\n",modeOnOff);
  Serial.printf("%i\n",moval);

 if (moval==LOW){
    wemoWrite(MYWEMO, LOW); //WEMO Functions
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Motion\nWEMO Off!\n"); //WEMO Display Functions
    display.display();
    //delay(200);
    //pirState = HIGH;
 //}
 } else {
    wemoWrite (MYWEMO, HIGH);// WEMO On FUnctions
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Motion\nWEMO On!\n"); //WEMO Display Functions
    display.display();
    delay(200);
    //pirState = LOW;
    // wemoTimer.startTimer(5000);
    // if(wemoTimer.isTimerReady()){
    // wemoWrite(MYWEMO, LOW);
  }
} else {
//Wemo Red button with added Display code-----------------------------------------
//delay(2000);
if (redButton.isClicked()) { // Added && modeOnOff) to fix button issue
//if (redButton.isClicked()) {
  display.clearDisplay(); //added display code
  display.setCursor(0,0); //added display code
  display.printf("Wemo#%i\n Manual\n Mode\n",MYWEMO);
  display.display(); //added display code
  wemoOnOff= !wemoOnOff;
   }
  if (wemoOnOff) {
  wemoWrite (MYWEMO, HIGH);
  //Serial.printf("Turning on Wemo# %i \n", MYWEMO);
  } else {
  wemoWrite(MYWEMO, LOW);
  //Serial.printf("Turning off Wemo# %i \n", MYWEMO);
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
if (millis()-lightTime>lightDelay) {
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
}
setHue(BULB,hueOnOff,colorNum,hueBright,255);
delay(100);

// NeoPixel Code with Black Button---------------------------------------------------------------------
if (blackButton.isClicked()) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.printf("NEOPIX\nOn\n");
  display.display();
//neoOnOff = digitalRead(D2);
//neoOnOff = !neoOnOff;
cycleCount++;
if (cycleCount=1) {
 for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
 pixel.setPixelColor (pixelAddr, rainbow[i]);

 delay(20); // needs to be turned on for NeoStrip SetPixelColor assignment
 }
  pixel.show (); // nothing changes until show ()
  i++;
  if (i>6){i=0;}
  delay(500);
  }

//   // End of NeoPixel Rainbow Code-------------------------
//   if (cycleCount=2) {
//   //pixel.clear (); // Clear Pixels----------------
//   for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
//   x=random(7);
//   mySeq[pixelAddr]=x;
//   pixel.setPixelColor(pixelAddr, rainbow[mySeq[pixelAddr]]);
//   pixel.show ();
//  }
//     delay(200);
//     for (i=0; i<PIXELCOUNT; i++) {
//     }
//     for (pixelAddr=0; pixelAddr < PIXELCOUNT; pixelAddr++) {
//     pixel.setPixelColor(pixelAddr, rainbow[mySeq[pixelAddr]]);
//     pixel.show ();
//  //pixel.clear ();
//    }
//   delay(10);
//   } 
//   if (cycleCount>2){cycleCount=0;}
 }
}


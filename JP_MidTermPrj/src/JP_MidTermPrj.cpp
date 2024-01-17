/* 
 * Project JP_MidTermPrj
 * Author: JP
 * Date: 01/17/2024 Adding more functionality to Motion Sensor function with Mulitple Hue's
 *  Tried to add the 3 WEMO into the OnOff motion Sequence- not workign properly.
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
#include "PyramidBitmap.h" // Custom Pyramid BMP
// PhotoDiode
const int LEDPIN = A2;
const int PHOTODIODE = A1;
int val;
// LED Sine Wave----------------------------------------
float value , n ;
float t;
float y;
float s;
// OLED
const int OLED_RESET=-1;
Adafruit_SSD1306 display(OLED_RESET);
//Hue
int BULB = 0;
int BULB4 = 4;
int BULB6 =6;
int color;
int colorNum;
int myHueSeq [] = {BULB,BULB4,BULB6};
int hueNumSeq;
//Millis time for lights
long unsigned int lightTime;
int lightDelay;
IoTTimer wemoTimer;
// Buttons
Button grayButton (D3); // Hue Lights
Button redButton (D5); // Wemo
Button myEncBtn (D17); // Hue Colors
Button blackButton (D4); // Neopixels
//Mode Switch
const int modeSwitch= D6; // Switches between Motion Sensor and Red Button OnOff for WEMO's
// Booleans for On Off
bool hueOnOff;
bool wemoOnOff;
bool neoOnOff;
bool movalOnOff;
bool modeOnOff;
bool changed;
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
// Encoder Button code
const int GREENBUTTONPIN=D17;
const int REDBUTTONPIN=D18;
int greenState,redState;
bool buttonState;
// Wemo
const int MYWEMO = 0;
const int MYWEMO3 = 3;
const int MYWEMO4 = 4;
int myWemoSeq [] = {MYWEMO,MYWEMO3,MYWEMO4};
int wemoNumSeq;
//NEOPIXEL
int ledState=LOW; 
const int PIXELCOUNT = 9; // Total number of NeoPixels might be 18 for the project!
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B); // declare object
const int neoPixBtn (D2);
int pixelAddr;
int colorCount;
int position;
int mySeq [PIXELCOUNT];
int x, temp, i, j, k;
int modeSeq;
//MotionSensor
const int MYPIR=0;
int motionPin=D11;
int pirState=LOW; //   t = millis() / 1000.0;
int moval;//   y = 128 * sin(2 * M_PI * 1/5.0 * t) + 128;
// Millis--------------------------
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
// WIfi-------------------------------------------------
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
// Photodiode and LED Sine Wave set up----------------------
pinMode(LEDPIN, OUTPUT);
pinMode(PHOTODIODE, INPUT);
n = 0;
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
// Motion Sensor PIR
pinMode(motionPin, INPUT);
// NeoPixel--------------------------------
pixel.begin ();
pixel.setBrightness (40); // bri is a value 0 - 255
pixel.show (); // initialize all off
// OLED Display functions------------------
Serial.begin(9600);
waitFor(Serial.isConnected,15000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.display();
delay(1000);
// Pyramid bitmap display-------------------------------
display.clearDisplay();
display.drawBitmap (0, 0, pyramidBitmap, 128, 64, WHITE);
display.display();
delay(2000);
// Invert Display
display.invertDisplay(true);
delay(1000);
display.invertDisplay(false);
delay(1000);
// Display Text
display.setTextSize(2);
display.setTextColor(WHITE);
// JP's Disply Project Credits
display.clearDisplay();
display.printf("JP Funk\nIoTMidTermSmart RoomController");//OLED display code WEMO OnOff-----------------
display.display(); // OLED Display Function for Red Button Wemo On Off-
// Hue Light--------------------------------------------
lightDelay=2000;
Serial.begin(9600);
waitFor(Serial.isConnected,15000);
BULB = 1;
BULB4 = 4;
BULB6 = 6;
// Encoder----------------------------------------------
pinMode(GREENBUTTONPIN, OUTPUT);
pinMode(REDBUTTONPIN, OUTPUT);
}
// loop() runs over and over again, as quickly as it can execute.
void loop() {
// The core of your code will likely live here.
  display.clearDisplay(); //OLED Master display code OnOff functions in Void Loop----
  display.setCursor(0,0);
  // PhotoDiode & Bright Red LED with Pulsing Sine Wave code------------------------
  val = analogRead(A1);
  if (val < 40){
//  display.clearDisplay(); 
  display.printf("Diode On%i\n",val);
  display.display();
   t = millis() / 1000.0;
   y = 128 * sin(2 * M_PI * 1/5.0 * t) + 128;
  analogWrite(LEDPIN, y);  //digitalWrite (LEDPIN, HIGH);
  } else {
  analogWrite(LEDPIN, LOW); //digitalWrite (LEDPIN, LOW);
//  display.clearDisplay();
  display.printf("Diode Off\n");
  display.display();
  }
  // Switch Code OnOff functions for Wemo RedButton and Motion Sensor w/ DIsplay Code--------------
  modeOnOff = digitalRead(modeSwitch);
  moval=digitalRead(motionPin);
  if (modeOnOff) {
  // Serial.printf("%i\n",modeOnOff);
  // Serial.printf("%i\n",moval);
   if (moval==LOW){  
    setHue (myHueSeq [hueNumSeq%3],FALSE,0,0,255);  //setHue(BULB,FALSE,0,0,255);   
    wemoWrite (myWemoSeq [wemoNumSeq%3], LOW); //Beginning New Motion Code Sequnce for WEMO Array--------------------
     // wemoWrite (myWemoSeq [wemoNumSeq%3], moval); // Motion Sensor OnOff with "moval" instead of LOW/HIGH
  //  display.clearDisplay();
    display.printf("Motion\nWEMO#%i Off!\n");//OLED display code Motion Sensor WEMO Off
    display.display(); // OLED Display Function for WEMO Button--------------------------------------------
    wemoNumSeq++;
    hueNumSeq++;
   } else {
    setHue (myHueSeq [hueNumSeq%3],TRUE,0,255,255); //setHue(BULB,TRUE,0,255,255);
    wemoWrite (myWemoSeq [wemoNumSeq%3], HIGH); //Motion Sensor WEMO On FUnctions
  //  display.clearDisplay();
    display.printf("Motion\nWEMO#%i On!\n");//OLED display code Motion Sensor WEMO ON
    display.display(); // OLED Display Function for WEMO Button--------------------------------------------
   } 
   hueOnOff==hueOnOff;
  } else {
  // Wemo Red button with added Display code-----------------------------------------
  //delay(2000);
  if (redButton.isClicked()) { // Added && modeOnOff) to fix button issue
  wemoOnOff= !wemoOnOff;
   }
  if (wemoOnOff) {
    wemoWrite (MYWEMO, HIGH); //WEMO 0 On Functions
    wemoWrite (MYWEMO3, HIGH); //WEMO 3 On Functions
    wemoWrite (MYWEMO4, HIGH); //WEMO 4 On Functions
  //  display.clearDisplay();
    display.printf("Wemo#%i\nManual On\n",MYWEMO);//OLED display code WEMO OnOff-----------------
    display.display(); // OLED Display Function for Red Button Wemo On Off--------------------------------------------
    // Serial.printf("Turning on Wemo# %i \n", MYWEMO);
    } else {
    wemoWrite (MYWEMO, LOW); //WEMO 0 Off Functions
    wemoWrite (MYWEMO3, LOW); //WEMO 3 Off Functions
    wemoWrite (MYWEMO4, HIGH); //WEMO 4 Off Functions
  //  display.clearDisplay();
    display.printf("Wemo#%i\nManual Off",MYWEMO);//OLED display code WEMO OnOff-----------------
    display.display(); // OLED Display Function for Red Button Wemo On Off--------------------------------------------
  // Serial.printf("Turning off Wemo# %i \n", MYWEMO);
   }
  }
  // Hue Button Functions with added Display code----------------------------------------------------------------------------------------
  if (grayButton.isClicked()) {
    changed = TRUE;
  //  display.clearDisplay();
    display.printf("Hue Light On/Off!\n");//OLED display code Hue Ligths OnOff-----------------
    display.display(); // OLED Display Function for Gray Button Hue On Off--------------------------------------------
    hueOnOff=!hueOnOff;
  }
  //Encoder Button to cycle through colors with added Display code-----------------------------------------------------------------------
    if (myEncBtn.isPressed()) {
  //  display.clearDisplay();
    display.printf("Set Bulb %iColor%06i\n",BULB%i,HueRainbow[color%7]);//OLED display code Hue Color-----------------
    display.display(); // OLED Display Function for Encoder Button--------------------------------------------
    colorNum=HueRainbow[color%7];
    color++;
    changed = TRUE;
    }
  // Encoder Funtions
      hueBright=myEnc.read();
      if(hueBright<=0){
      hueBright=0;
      myEnc.write(0);
      }
      if(hueBright>=255){
      hueBright=255;
      myEnc.write(255);
      }
  // Encoder button with added Display code-----------------------------------------------------------------------------------------------
    if (hueBright!= prevenc){
  //  display.clearDisplay(); //added display code
    display.printf("BrightnessValue %i\n",hueBright);//OLED printf code Hue Brightness Encoder Potentiometer-----------------
    display.display(); // OLED Display Function for Encoder Button--------------------------------------------
    prevenc = hueBright; 
    changed = TRUE;
    }
    if(changed) {
    setHue(BULB,hueOnOff,colorNum,hueBright,255);
    setHue(BULB6,hueOnOff,colorNum,hueBright,255); // Temporarily Commented out Hue BULB 6 as it slows the code
    changed=FALSE;
  }

  // NeoPixel Code with Black Button---------------------------------------------------------
  if (blackButton.isClicked()) {
    modeSeq++;
   // display.clearDisplay();
    display.printf("NEOPIXEL\nModeSeq\n");//added OLED display code -----------------
    display.display(); // OLED Display Function for NeoPixel Button--------------------------------------------
    }
    if (modeSeq%4==1) {  //Mode Sequence for NeopIxels---------------------------------------
   // display.clearDisplay();
    display.printf("NEOPIXEL\nRainbow On");//added OLED display code -----------------
    display.display(); // OLED Display Function for NeoPixel Button--------------------------------------------
    // NeoPixel Rainbow functions
    for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
    pixel.setPixelColor (pixelAddr, rainbow[i]);
    delay(20); // needs to be turned on for NeoStrip SetPixelColor assignment
    }
    pixel.show (); // nothing changes until show ()
    i++;
    if (i>6){i=0;}
    delay(100); 
  }
  // NeoPixel Random Code----------------------------------------------------------
  if (modeSeq%4==2) {
  //pixel.clear (); // Clear Pixels----------------
   // display.clearDisplay();
    display.printf("NEOPIXEL\nRandom On\n");//added OLED display code -----------------
    display.display(); // OLED Display Function for NeoPixel Button--------------------------------------------
    // NeoPixel Random Color Functions
    for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
    x=random(7);
    mySeq[pixelAddr]=x;
    pixel.setPixelColor(pixelAddr, rainbow[mySeq[pixelAddr]]);
    pixel.show ();
    }
    delay(200);
 }
  if (modeSeq%4==3) {
  // pixel.clear (); // Clear Pixels----------------
   // display.clearDisplay();
    display.printf("NEOPIXEL\nColor On\n");//added OLED display code -----------------
    display.display(); // OLED Display Function for Encoder Button--------------------------------------------
    for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
    pixel.setPixelColor (pixelAddr, rainbow[i]);
    delay(100); // needs to be turned on for NeoStrip SetPixelColor assignment
    }
    pixel.show (); // nothing changes until show ()
    i++;
    if (i>6) {i=0;}
  //delay(300);
  }
 // new code addition for turnign off Neoplixels
  if (modeSeq%4==4) {
 // display.clearDisplay();
  display.printf("NEOPIXEL\nOff\n");//added OLED display code -----------------
  display.display(); // OLED Display Function for Encoder Button--------------------------------------------
  pixel.clear (); // Clear Pixels----------------
  }
  //display.display(); // OLED Display Function Temp-------------------------------------------
}


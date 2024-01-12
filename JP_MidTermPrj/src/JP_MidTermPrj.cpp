/* 
 * Project JP_MidTermPrj
 * Author: JP
 * Date: 01/11/2024
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
const int PIXELCOUNT = 8; // Total number of NeoPixels might be 18 for the project!
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
int k;
int modeSeq;
//MotionSensor
const int MYPIR=0;
int motionPin=D11;
int pirState=LOW; 
int moval;
// RGB LED Sine Wave---------Not Using-------------------------------
float value , n ;
float t;
float y;
float s;
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
pixel.setBrightness (40); // bri is a value 0 - 255
pixel.show (); // initialize all off
//OLED Display functions------------------
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
}
// loop() runs over and over again, as quickly as it can execute.
void loop() {
// The core of your code will likely live here.
  display.clearDisplay(); //OLED Master display code OnOff functions in Void Loop----
  display.setCursor(0,0);
  // PhotoDiode LED-------------------------
  val = analogRead(A1);
  Serial.printf("photodiode %i \n",val);
  if (val < 40){ 
  digitalWrite(LEDPIN, HIGH);
  } else {
  digitalWrite (LEDPIN, LOW);
  }
  // Switch Code OnOff functions for Wemo RedButton and Motion Sensor w/ DIsplay Code--------------
  modeOnOff = digitalRead(modeSwitch);
  moval=digitalRead(motionPin);
  if (modeOnOff) {
  // Serial.printf("%i\n",modeOnOff);
  // Serial.printf("%i\n",moval);
   if (moval==LOW){
    wemoWrite (MYWEMO, LOW); //WEMO Functions
    display.printf("Motion\nWEMO Off!\n");//OLED display code Motion Sensor WEMO Off-----------------
    display.display(); // OLED Display Function for WEMO Button--------------------------------------------
  } else {
    wemoWrite (MYWEMO, HIGH);// WEMO On FUnctions
    display.printf("Motion\nWEMO On!\n");//OLED display code Motion Sensor WEMO ON-----------------
    display.display(); // OLED Display Function for WEMO Button--------------------------------------------
    //delay(200);
  }
  //display.display(); // OLED Display Function for Red Button Wemo On Off--------------------------------------------
  } else {
//Wemo Red button with added Display code-----------------------------------------
//delay(2000);
 if (redButton.isClicked()) { // Added && modeOnOff) to fix button issue
  wemoOnOff= !wemoOnOff;
   }
 if (wemoOnOff) {
  wemoWrite (MYWEMO, HIGH);
  display.printf("Wemo#%i\nManual On\n",MYWEMO);//OLED display code WEMO OnOff-----------------
  display.display(); // OLED Display Function for Red Button Wemo On Off--------------------------------------------
  //Serial.printf("Turning on Wemo# %i \n", MYWEMO);
   } else {
  wemoWrite(MYWEMO, LOW);
  display.printf("Wemo#%i    Manual Off",MYWEMO);//OLED display code WEMO OnOff-----------------
  display.display(); // OLED Display Function for Red Button Wemo On Off--------------------------------------------
  //Serial.printf("Turning off Wemo# %i \n", MYWEMO);
   }
  }
// Hue Button Functions with added Display code----------------------------------------------------------------------------------------
 if (grayButton.isClicked()) {
  display.printf("Hue Light On/Off!\n");//OLED display code Hue Ligths OnOff-----------------
  display.display(); // OLED Display Function for Gray Button Hue On Off--------------------------------------------
  hueOnOff=!hueOnOff;
  }
//Encoder Button to cycle through colors with added Display code-----------------------------------------------------------------------
 if (myEncBtn.isPressed()) {
  display.printf("Set Bulb %iColor%06i\n",BULB,HueRainbow[color%7]);//OLED display code Hue Color-----------------
  display.display(); // OLED Display Function for Encoder Button--------------------------------------------
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
//Encoder button with added Display code-----------------------------------------------------------------------------------------------
  if (hueBright!= prevenc){
  display.clearDisplay(); //added display code
  display.setCursor(0,0); //added display code
  display.printf("BrightnessValue %i\n",hueBright);//OLED printf code Hue Brightness Encoder Potentiometer-----------------
  display.display(); // OLED Display Function for Encoder Button--------------------------------------------
  prevenc = hueBright;
 }
}
setHue(BULB,hueOnOff,colorNum,hueBright,255);
delay(100); 
// NeoPixel Code with Black Button---------------------------------------------------------
if (blackButton.isClicked()) {
  modeSeq++;
  display.printf("NEOPIXEL\nModeSeq\n");//added OLED display code -----------------
  display.display(); // OLED Display Function for NeoPixel Button--------------------------------------------
}
  if (modeSeq%4==1) {  //Mode Sequence for NeopIxels---------------------------------------
  display.printf("NEOPIXEL\nRainbow On\n");//added OLED display code -----------------
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
   //NeoPixel Random Code----------------------------------------------------------
  if (modeSeq%4==2) {
  //pixel.clear (); // Clear Pixels----------------
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
  //if (modeSeq>2){modeSeq=0;}
    if (modeSeq%4==3) {
  //pixel.clear (); // Clear Pixels----------------
  display.printf("NEOPIXEL\nColor On\n");//added OLED display code -----------------
  display.display(); // OLED Display Function for Encoder Button--------------------------------------------
  for (pixelAddr =0; pixelAddr <PIXELCOUNT; pixelAddr++) {
  pixel.setPixelColor (pixelAddr, rainbow[i]);
  delay(20); // needs to be turned on for NeoStrip SetPixelColor assignment
  }
  pixel.show (); // nothing changes until show ()
  i++;
  if (i>6){i=0;}
  delay(100);
  }
  //display.display(); // OLED Display Function Temp-------------------------------------------
}
 // SIne Wave Functins for LED/NeoPixels-------------------
//   t = millis() / 1000.0;
//   y = 128 * sin(2 * M_PI * 1/5.0 * t) + 128;
//  analogWrite (LEDPIN, y);


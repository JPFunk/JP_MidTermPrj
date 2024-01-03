/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Colors.h"
#include "neopixel.h"

 const int PIXELCOUNT = 16; // Total number of NeoPixels
 Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B); // declare object
 int pixelAddr;
 int colorCount;
 int mySeq [PIXELCOUNT];
 int x;
 int temp;
 int i;
 int j;
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
// SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
 //PIXELCOUNT=p;
 pixel.begin ();
 pixel.setBrightness (64); // bri is a value 0 - 255
 pixel.clear ();
 pixel.show (); // initialize all off
 colorCount=0;
 pixelAddr=0;
 random (0,4); // LED assignment 0,1,2,3,4
 rainbow[0,1,2,3,4];
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
 //delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!
for (pixelAddr=0; pixelAddr < PIXELCOUNT; pixelAddr++) {
  x=random(7);
  mySeq[pixelAddr]=x;
//(p=0; p=<46, p=p+1)
//for (colorCount = 0; colorCount <= 6; colorCount++) {
  // The core of your code will likely live here.
 pixel.setPixelColor(pixelAddr, rainbow[mySeq[pixelAddr]]);
 pixel.show ();
 //pixel.clear ();
 //delay(100); // needs to be turned on for NeoStrip SetPixelColor assignment
  }
  delay(2000);

  for (i=0; i<PIXELCOUNT; i++) {
  //for (j=0; j<PIXELCOUNT-1; j++) {
  //if(mySeq[j]>mySeq[j+1]) {
  //temp=mySeq[j];
  //mySeq[j]=mySeq[j+1];
  // mySeq[j+1]=temp;
  }

  for (pixelAddr=0; pixelAddr < PIXELCOUNT; pixelAddr++) {
 pixel.setPixelColor(pixelAddr, rainbow[mySeq[pixelAddr]]);
 pixel.show ();
 //pixel.clear ();
  }
 delay(10); // needs to be turned on for NeoStrip SetPixelColor assignment
  }

 //--------------------------------------------------------------------------------- 
  #ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define SWITCH     4
#define PIN        6
#define NUMPIXELS 12

byte counter     = 1;
byte lastSWITCH;

unsigned long switchMillis;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//*****************************************************************
void setup()
{
  pixels.begin();
  pixels.clear();
  pixels.show();

  pinMode(SWITCH, INPUT_PULLUP);

}

//*****************************************************************
void loop()
{
  checkSwitches();

} //END of loop()

//*****************************************************************
void checkSwitches()
{
  //check for a switch change every 250ms (1/4 second)
  if (millis() - switchMillis < 250)
  {
    //it is not time to check our switches yet
    return;
  }

  //it is time to check our switches
  //restart the timer
  switchMillis = millis();

  byte currentState = digitalRead(SWITCH);

  //has this switch changed state ?
  if (lastSWITCH != currentState)
  {
    //the switch has change state, save this new state
    lastSWITCH = currentState;

    //has the switch been pushed (i.e. closed)
    if (currentState == LOW)
    {
      //push ON, push OFF switch action
      counter = 1 - counter;

      //do we turn on the pixels ?
      if (counter == 0)
      {
        for (int i = 0; i < NUMPIXELS; i++)
        {
          pixels.setPixelColor(i, pixels.Color(75, 0, 130));
        }

        pixels.show();
      }

      //turn off the pixels
      else
      {
        pixels.clear();

        pixels.show();
      }

    } //END of if (currentState == HIGH)

  } //END of  if (lastOnSwitch != currentState)

} //END of checkSwiches()

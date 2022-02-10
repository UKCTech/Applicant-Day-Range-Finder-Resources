// June 2019 
// Ultrasonic based range finder based on NANO. Has an 128 x 64 I2C OLED LCD display, buzzer, HC-04 U/S module.
// 
//Arduino IDE > Tools> select Nano. Select ATMega328p (old bootloader). Select the correct COM port. Select Programmer AVRISP mkII
// Add libraries if you haven't done so already:
// Arduino IDE > Sketch, Include Library, add .ZIP Library.

// Rev_1 : 25/03/2020 Graphics Resolution update

#include <Wire.h>                 // include the Wire (I2C) library - find it in the library manager
#include <NewTone.h>              // include the Newtone library by Tim Eckel   -  https://bitbucket.org/teckel12/arduino-new-tone/wiki/Home
#include <NewPing.h>              // include the Newping library by Tim Eckel - find it in the library manager
#include <Adafruit_GFX.h>         // include the Adafruit graphics library - find it in the library manager
#include <Adafruit_SSD1306.h>     // include the Adafruit OLED display library - find it in the library manager

#define SCREEN_WIDTH 128          // OLED display width, in pixels. We use a 0.96' 128x64 pixels
#define SCREEN_HEIGHT 64          // OLED display height, in pixels

const int Triggerpin = 2;    // Ultrasound module trigger pin connection on the Nano
const int Echopin = 3;       // Ultrasound module echo pin connection on the Nano
const int Status_LED = 5;    // LED connected to the Nano pin

#define OLED_RESET 6 //not presently used, but needed to make the code compile without errors.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "UKCLOGO.h"    // Include the logo file that is displayed at startup.
 
NewPing sonar (Triggerpin, Echopin, 300);  // set max distance in cm

void setup()              //run once to setup
{
Serial.begin(9600);             // start the serial port so we can se serial data
pinMode(Status_LED,OUTPUT);     // set pinmodes for all the LED
pinMode(4,OUTPUT);               // define the pin for the buzzer as an output
digitalWrite(Status_LED, LOW);    // Force the LED to be off when the Nano starts up.

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)  OR 0x3C//// Don't think this actually works...just use 0X3C ??
display.clearDisplay();                     // Clear the display of any junk at startup

// --------------- display.drawBitmap(topLeftX, topLeftY, imageData, width, height, color)

display.drawBitmap(-30,0,UKCLOGO,128,64,WHITE, BLACK); // Draw the bitmap logo on the display at a certain position
display.display();                                      // Display the contents on the display
delay(5000);
display.clearDisplay();
}

void loop()                                 // loop all the time
{
  int uS = sonar.ping_median(5);              // filter the readings to make it more stable - A feature of the newping library
  int dist = (10*(uS / US_ROUNDTRIP_CM));     // Convert ping time to distance in mm and print result (0 = outside set distance range)

  Serial.print("Distance mm: ");              // Serial port print text
  Serial.println(dist);                        // serial port print the distance

  display.setTextSize(2);   // Text size 1, 2, 3 etc
  display.setTextColor(WHITE);    //text colour
  display.setCursor(15,10); // position of text on display horiz/ vert. 15,10
  display.println(dist);  // Print the distance to the display
  display.setTextSize(2);   // Text size 1, 2, 3 etc
  display.setTextColor(WHITE);
  display.setCursor(0,0);  // 0,0
  display.println("        mm");  // show distance units are in mm
  display.display();
  display.clearDisplay();

  if (dist > 101 & dist < 1500)  // Set tone when between these limits. units are in mm
  {
    NewTone(4,800,65);   // play tone on pin D4
    digitalWrite(Status_LED, HIGH);  // LED turned on
    delay(dist/10);        // alter timing (delay) of the tone as distance changes. The dist is divided by a number to make the delay shorter and thus respond quicker
    digitalWrite(Status_LED, LOW);  // LED turned off
  }
  
  else if(dist >= 1 && dist <= 100)   // Set tone when between these limits. units are in mm
  {
    NewTone(4,800);              // play continous tone on pin D4
    digitalWrite(Status_LED, HIGH);  // turn LED on
  }

  else        //Any other distance, then set all the LEDs to OFF
  {
    noNewTone(4);   //Otherwise no tone on pin D1
    digitalWrite(Status_LED, LOW);  // turn LED off
  }
  
  delay(5);                      // add a delay for safety
}

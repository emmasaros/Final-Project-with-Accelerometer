//Accelerometer Part
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

//SD Music Part
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav           playWav1;
AudioOutputAnalog      audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection          patchCord2(playWav1, 1, audioOutput, 1);

#define SDCARD_CS_PIN   BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13


//NeoPixel Part
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN            14
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW);
int delayval = 50;



float accelVector;
float lastAccelVector;
float change;
float tenCounterX;
float xAverage;
float tenCounterY;
float yAverage;
float tenCounterZ;
float zAverage;
int ledPin = 32; // set up variable for first trigger
int ledPin1 = 31;
int ledPin2 = 30;
float peakValue;
unsigned long lastStepTime = 0;
int stepLength = 500;
//abs value
//every 5 seconds check that, if that value light led
float lastCheck;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pixels.begin(); // This initializes the NeoPixel library.

  AudioMemory(8);
  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);

  if (!(SD.begin(BUILTIN_SDCARD))) {
      // stop here, but print a message repetitively
      while (1) {
        Serial.println("Unable to access the SD card");
        delay(500);
      }
    }
    
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

}



void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event;
  accel.getEvent(&event);

  for (int i = 0; i < 10000; i++) {
    tenCounterX += event.acceleration.x;
    tenCounterY += event.acceleration.y;
    tenCounterZ += event.acceleration.z;
  }

  xAverage = tenCounterX / 10000.0;
  tenCounterX = 0;

  yAverage = tenCounterY / 10000.0;
  tenCounterY = 0;

  zAverage = tenCounterZ / 10000.0;
  tenCounterZ = 0;



  lastAccelVector = accelVector;
  accelVector = sqrt(pow(xAverage, 2) + pow(yAverage, 2) + pow(zAverage, 2));
  change = 10 * (accelVector - lastAccelVector);

  if (change < 3 && change > -3) {
    change = 0;
  }

  Serial.println(change);

  if (millis() > lastStepTime + stepLength) {

    //human is still
    if (peakValue < 3) {
        for(int i=0;i<NUMPIXELS;i++){
  
            // pixels.Color takes GRB values, from 0,0,0 up to 255,255,255
            pixels.setPixelColor(i, pixels.Color(0,100,0)); // Moderately bright green color.
        
            pixels.show(); // This sends the updated pixel color to the hardware.
        
            delay(delayval); // Delay for a period of time (in milliseconds).
  
    }
        if (playWav1.isPlaying() == false) {
        playFile("SDTEST3.WAV");
      }
    }

    //human is walking
    //peakValue check
    if (peakValue >= 3 && peakValue <= 25) {
      for(int i=0;i<NUMPIXELS;i++){

          // pixels.Color takes GRB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(i, pixels.Color(50,50,0)); // Moderately bright green color.
      
          pixels.show(); // This sends the updated pixel color to the hardware.
      
          delay(delayval); // Delay for a period of time (in milliseconds).
      
        }
        if (playWav1.isPlaying() == true) {
          playFile("SDTEST1.WAV");
        }
    }

    //human is running
    //peakValue check
    if (peakValue > 25) {
        for(int i=0;i<NUMPIXELS;i++){

          // pixels.Color takes GRB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(i, pixels.Color(0,0,150)); // Moderately bright green color.
      
          pixels.show(); // This sends the updated pixel color to the hardware.
      
          delay(delayval); // Delay for a period of time (in milliseconds).
      
        }
        if (playWav1.isPlaying() == true) {
          playFile("SDTEST2.WAV");
        }
    }


    lastStepTime = millis(); //lastStepTime is now right now
    peakValue = abs(change);

  }

  if (abs(change) > peakValue) {
    peakValue = abs(change);
  }




//start reading button?
//switch to neopixels
//perfboard and plastic box

}

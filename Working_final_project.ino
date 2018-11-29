#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

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
  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);

  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event;
  accel.getEvent(&event);

  for (int i = 0; i < 10000; i++) {
    tenCounterX += event.acceleration.x;
    tenCounterY += event.acceleration.y;
    tenCounterZ += event.acceleration.z;
    //    delay(1);
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

    if (peakValue < 3) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
    }

    //peakValue check
    if (peakValue >= 3 && peakValue <= 25) {
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
    }
    //peakValue check
    if (peakValue > 25) {
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
    }


    lastStepTime = millis(); //lastStepTime is now right now
    peakValue = abs(change);

  }

  if (abs(change) > peakValue) {
    peakValue = abs(change);
  }




//start reading button?
//sd card insertion and application
//add audio jack
//switch to neopixels
//perfboard and plastic box

}

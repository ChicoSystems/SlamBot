//** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include "DistanceSensorThread.h"
#include "TimerOne.h"
#include "RedBot.h"
#include "CompassThread.h"
#include "RedBotSoftwareSerial.h"

#include <Wire.h>

// Create an alias for our pins
#define BEEPER A7
#define BUMPER A6

// Instantiate a software serial port. Note that the regular Arduino
//  SoftwareSerial won't work! It steals resources that the sensor
//  inputs on the RedBot need. Also note that the RedBot version has
//  no input options- it automatically connects to the pins used for
//  the onboard XBee header.
//RedBotSoftwareSerial swsp;

// Instantiate the motors.
RedBotMotor motors;

// Instantiate our encoder. 
RedBotEncoder encoder = RedBotEncoder(11, 10); // left, right

//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
CompassThread compass = CompassThread(); //we're not using this as a thread here
ThreadController controller = ThreadController();
RedBotBumper lBumper(BUMPER, &bump);

float goalHeading = 0;
boolean bumped = false;

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
 // Serial.print(" timerCallback() ");
  controller.run(); 
}

void setup(){
  Serial.begin(9600); 
  Serial.println("SLAMBOT");
  
  tone(BEEPER, 2600, 750);
  
  // Send out a hello via the XBee, if one is present.
 // swsp.begin(9600);
  //swsp.print("Hello, world");
  Serial.println("initializing compass");
  compass.init();
  Serial.println("compajss initialized");
  //setup pins to distanceSensor
  distanceSensor.fPinWrite = A2;
  distanceSensor.fPinRead = A3;
  distanceSensor.rPinWrite = 3; 
  distanceSensor.rPinRead = 3;
  distanceSensor.bPinWrite = 9;
  distanceSensor.bPinRead = 9;
  distanceSensor.lPinWrite = A0;
  distanceSensor.lPinRead = A1;
  
  //set the interval that the class will run at.
  distanceSensor.setInterval(100);//in milliseconds
  
  Serial.println("adding distance sensor");
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  Serial.println("initializing timer");
  //Initialize the timer used for multithreading.
  Timer1.initialize(500000); //in microseconds
  Timer1.attachInterrupt(timerCallback);
  Serial.println("timer initialized");
  
  //encoders don't work unless we power motors first.
  motors.brake();
        motors.drive(255);
        delay(50);
        motors.stop();
  
  encoder.clearEnc(BOTH);
}

void loop(){
  //swsp.println(distanceSensor.f);
  // Here is where the Threads are processed
  delay(500);
  Serial.print(" COMPASS:");
  Serial.print(compass.getHeading());
  Serial.print(" F:");
  Serial.print(distanceSensor.f);
  Serial.print(" R:");
  Serial.print(distanceSensor.r);
  Serial.print(" B:");
  Serial.print(distanceSensor.b);
  Serial.print(" L:");
  Serial.print(distanceSensor.l);
  Serial.print(" BUMPED:");
  Serial.print(bumped);
  Serial.print(" L-ENCODER:");
  Serial.print(encoder.getTicks(LEFT)); 
  Serial.print(" R-ENCODER:");
  Serial.println(encoder.getTicks(RIGHT)); 
  
  
  if(bumped)bumped = !bumped;
  //More Thread Processing done, why break it in half?
  delay(500);
}

void bump(){
  
  bumped = true;
  tone(BEEPER, 150, 750);
}


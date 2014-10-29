//** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include "DistanceSensorThread.h"
#include "TimerOne.h"
#include "RedBot.h"
#include "CompassThread.h"
#include "RedBotSoftwareSerial.h"

#include <Wire.h>

// Instantiate a software serial port. Note that the regular Arduino
//  SoftwareSerial won't work! It steals resources that the sensor
//  inputs on the RedBot need. Also note that the RedBot version has
//  no input options- it automatically connects to the pins used for
//  the onboard XBee header.
//RedBotSoftwareSerial swsp;



//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
CompassThread compass = CompassThread(); //we're not using this as a thread here
ThreadController controller = ThreadController();

float goalHeading = 0;

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
 // Serial.print(" timerCallback() ");
  controller.run(); 
}

void setup(){
  Serial.begin(9600); 
  Serial.println("SLAMBOT");
  
  // Send out a hello via the XBee, if one is present.
 // swsp.begin(9600);
  //swsp.print("Hello, world");
  
  compass.init();
  //setup pins to distanceSensor
  distanceSensor.fPinWrite = A2;
  distanceSensor.fPinRead = A3;
  distanceSensor.rPinWrite = 10; 
  distanceSensor.rPinRead = 10;
  distanceSensor.bPinWrite = 11;
  distanceSensor.bPinRead = 11;
  distanceSensor.lPinWrite = A0;
  distanceSensor.lPinRead = A1;
  
  //set the interval that the class will run at.
  distanceSensor.setInterval(100);//in milliseconds
  
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  
  //Initialize the timer used for multithreading.
  Timer1.initialize(500000); //in microseconds
  Timer1.attachInterrupt(timerCallback);
}

void loop(){
  //swsp.println(distanceSensor.f);
  // Here is where the Threads are processed
  delay(500);
  Serial.print(" COMPASSer: ");
  Serial.print(compass.getHeading());
  Serial.print(" F: ");
  Serial.print(distanceSensor.f);
  Serial.print(" R: ");
  Serial.print(distanceSensor.r);
  Serial.print(" B: ");
  Serial.print(distanceSensor.b);
  Serial.print(" L: ");
  Serial.println(distanceSensor.l);
  //More Thread Processing done, why break it in half?
  delay(500);
}


//** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include "DistanceSensorThread.h"
#include "TimerOne.h"
#include "RedBot.h"
#include "CompassThread.h"

#include <Wire.h>

//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
CompassThread compass = CompassThread(); //we're not using this as a thread here
ThreadController controller = ThreadController();

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
 // Serial.print(" timerCallback() ");
  controller.run(); 
}

void setup(){
  Serial.begin(9600); 
  Serial.println("SETUP()");
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
  // Here is where the Threads are processed
  delay(500);
  Serial.print(" COMPASS: ");
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

/**
  * DistanceSensorThread - An object that keeps track of DistanceSensorValues
  * Author - Isaac Assegai - https://github.com/itravers
  * When this object runs it pings 4 different sonar sensors one after
  * the other, keeping track of the lastest distance for each ping.
  * This is also where we will introduce filters and smoothing to work
  * around sensor error.
  * This class utilizes the ArduinoThread library allowing compass
  * values to be read and stored in an asynchronous order with the rest 
  * of the code in the project which can be found
  * here: https://github.com/ivanseidel/ArduinoThread 
  */

#ifndef DistanceSensorThread_h
#define DistanceSensorThread_h
#include "Thread.h"
#include "ThreadController.h"

class DistanceSensorThread: public Thread{
  public:
    float f, r, b, l; // Stores the value of the pings.
    int fPinWrite, fPinRead, rPinWrite, rPinRead, // Tells us which pins to hook up to the four sensors.
        bPinWrite, bPinRead, lPinWrite, lPinRead; // NOTE: These need to be set manually by the calling code
    
    DistanceSensorThread(){
      //Let's not set anything up in the constructor.
    }
    
    /**
      * Used by the multi-threading library to read all
      * four distances at a specific interval.
      */
    void run(){
     // Serial.print(" run() ");
      f = ping(0); // FRONT
      r = ping(1); // RIGHT
      b = ping(2); // BACK
      l = ping(3); // LEFT
      runned(); // Tell the multi-threading library that we are done running this time.
    }
     
    /**
      * Used to get the distance from each individual sonar sensor.
      * directions are as follows - 0 == FRONT, 1 == RIGHT, 2 == BACK, 3 == LEFT
      */ 
    float ping(int direction){
      float duration; // The time it took for the signal to reflect back to us
      long cm = 0; // The distance reading in centimeters.
      int readPin = 0; // The pin that will be used to read the signal reflection.
      int writePin = 0; // The pin that will be used to write the signal.
      
      noInterrupts(); //disable multi-thread interrupts when pinging
        if(direction == 0){ //Here we are going to set the correct pins for the current sensor
          readPin = fPinRead; // FRONT
          writePin = fPinWrite; 
        }else if(direction == 1){
          readPin = rPinRead; // RIGHT
          writePin = rPinWrite;
        }else if(direction == 2){
          readPin = bPinRead; // BACK
          writePin= bPinWrite; 
        }else if(direction == 3){
          readPin = lPinRead; // LEFT
          writePin = lPinWrite; 
        }
        
        /* Ping is triggered by a high pulse of more than 2 microseconds
           gives a short LOW pulse beforehand to ensure clean High signal. */
        pinMode(writePin, OUTPUT);
        digitalWrite(writePin, LOW);
        delayMicroseconds(2);
        digitalWrite(writePin, HIGH);
        delayMicroseconds(5);
        digitalWrite(writePin, LOW);
               
        /* The same pin is used to read the signal from the Parallax Ping Sensors
           An extra pin is used  is used to read the signal from the HC-SR04 Sonar Sensors
           We are using a timeout here to limit the amount of time the HC-SR04's
           Spend waiting for a return pulse. */
        pinMode(readPin, INPUT);
        duration = pulseIn(readPin, HIGH, 32000);
      interrupts(); // This will enable the interrupts egain. DO NOT FORGET!
      
      cm = microsecondsToCentimeters(duration); // Convert time to distance
      return cm;
    }
    
    /**
      * Converts microseconds to inches.
      * Sound from the sonar sensors travels at ~ 73.746 microseconds
      * in an inch. Approx 1130 ft/s. We divide that by 2
      * to accound for both the out-bound and return trips.
     */
    long microsecondsToInches(long microseconds){
      return microseconds / 74 / 2;
    }
    
    /**
      * Converts microseconds to feet.
     */
    long microsecondsToFeet(long microseconds){
      return microsecondsToInches(microseconds)/12;
    }
    
    /**
      * Converts microseconds to centimeters.
      * Approx 29 microseconds per centimeter. / 2
     */
    long microsecondsToCentimeters(long microseconds){
      return microseconds / 29 / 2;
    }
};
#endif


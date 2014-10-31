/**
  * CompassThread - An object that keeps track of compass values.
  * Author - Isaac Assegai - https://github.com/itravers
  * This class utilizes the ArduinoThread library allowing compass
  * values to be read and stored in an asynchronous order with the rest 
  * of the code in the project which can be found
  * here: https://github.com/ivanseidel/ArduinoThread
  * This class also utilizes the LSM303 Library to allow us to get
  * easy readings from the LSM303 compass. This library can be found
  * here:  https://github.com/pololu/lsm303-arduino
 */
 
#ifndef CompassThread_h
#define CompassThread_h
#include "Thread.h"
#include "ThreadController.h"
#include "LSM303.h"
#include <Wire.h>

class CompassThread: public Thread{
  public:
    float heading; // The most recent heading read from the compass.
    LSM303 compass; // The compass object defined in LSM303H
    
    CompassThread(){
      // We'd rather initialize everything in the init function.
    }
    
    /**
      * We need to initialize the compass and set up it's calibration info
     */
    void init(){
      Serial.println("CompassThread()");
      Wire.begin(); // Required for LSM303
      compass.init();
      compass.enableDefault();
       
      /* These min/max values for the x, y and z axis of the LSM303 compass
        were figured out using using examples from the LSM303 library */
      compass.m_min = (LSM303::vector<int16_t>){-784, -623, -555};
      compass.m_max = (LSM303::vector<int16_t>){+322, +517, +489};
      compass.read();
      heading = compass.heading(); 
    }
    
    /**
      * Returns the current heading of the compass.
     */
    float getHeading(){
      compass.read();
      heading = compass.heading();
      Serial.println(heading);
      return heading;  
    }
    
    /**
      * Used by the multi-threading library to read the compass
      * at a specific interval.
      */
    void run(){
      heading = compass.heading();
      runned(); // Used by the multi-threading library 
    }
};
#endif


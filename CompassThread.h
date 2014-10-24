#include "Thread.h"
#include "ThreadController.h"
#include "LSM303.h"
#include <math.h>

/**
  * CompassThread - An object that keeps track of compass values.
  * Author - Isaac Assegai
  * This class utilitzed the ArduinoThread library allowing compass
  * values to be read and stored in an asynchronous order with the rest 
  * of the code in the project
 */
class CompassThread: public Thread{
  public:
    float heading; //The most recent heading read from the compass.
    LSM303 compass; 
    
    CompassThread(){
       //Serial.println("constructor()");
       compass.init();
       compass.enableDefault();
       
       /*
      These min/max values for the x, y and z axis of the LSM303 compass
      were figured out using using examples from the LSM303 library, who's
      source can be found here: https://github.com/pololu/lsm303-arduino
      */
      compass.m_min = (LSM303::vector<int16_t>){-784, -623, -555};
      compass.m_max = (LSM303::vector<int16_t>){+322, +517, +489};
       heading = -1;
    }
    
    //Used by the multi-threading library
    void run(){
     // Serial.print(" run() ");
      
      runned();
    }
};

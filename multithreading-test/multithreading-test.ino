/** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include <TimerOne.h>

class DistanceSensorThread: public Thread{
  public:
    //Stores the value of the pings
    float f, r, b, l; 
    
    //Tells us which pins to hook up to the four sensors.
    int fPinWrite, fPinRead, rPinWrite, rPinRead,
        bPinWrite, bPinRead, lPinWrite, lPinRead;
        
    //Constants that can be used to denote which sensor
    const int FRONT = 0;
    const int RIGHT = 1;
    const int BACK = 2;
    const int LEFT = 3;
    
    //Used by the multi-threading library
    void run(){
      f = ping(FRONT);
      r = ping(RIGHT);
      b = ping(BACK);
      l = ping(LEFT); 
      runned();
    }
};

//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
ThreadController controller = ThreadController();

//The function that the timer will call for the DistanceSensor.
void DistanceSensorCallback(){
  distanceSensor.run(); 
}

void setup(){
  Serial.begin(9600); 
  distanceSensor.fPinWrite = A2;
  distanceSensor.rPin = 
  distanceSensor.bPin =
  distanceSensor.lPin =
}



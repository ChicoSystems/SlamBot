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
  distanceSensor.setInterval(250);
  
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  
  //Initialize the timer used for multithreading.
  Timer1.initialize(5000);
  Timer1.attachInterrupt(timerCallback);
  Timer1.start();
}



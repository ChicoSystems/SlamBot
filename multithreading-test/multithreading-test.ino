/** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include <TimerOne.h>

class DistanceSensorThread: public Thread{
  public:
    float f, r, b, l;
    const int FRONT = 0;
    const int RIGHT = 1;
    const int BACK = 2;
    const int LEFT = 3;
    
    void run(){
      f = ping(FRONT);
      r = ping(RIGHT);
      b = ping(BACK);
      l = ping(LEFT); 
      runned();
    }
}


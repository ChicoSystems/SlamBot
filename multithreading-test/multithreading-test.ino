/** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include <TimerOne.h>
#include <RedBot.h>

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
      
      long ping(int direction){
        long duration;
        long cm = 0;
        noInterrupts(); //disable multi-thread interrupts when pinging
         switch(direction){
            case FRONT:
              //ping is triggered by a high pulse of more than 2 microseconds
               //gives a short LOW pulse beforehand to ensure clean High signal
               pinMode(fPinWrite, OUTPUT);
               digitalWrite(fPinWrite, LOW);
               delayMicroseconds(2);
               digitalWrite(fPinWrite, HIGH);
               delayMicroseconds(5);
               digitalWrite(fPinWrite, LOW);
               
               //the same pin is used to read the signal from the ping
               //a high pulse whos duration is the time from the sending
               //of the ping to the reception of its echo off of an object
               pinMode(fPinRead, INPUT);
               duration = pulseIn(fPinRead, HIGH, 8000);
            break;
            case RIGHT:
              //ping is triggered by a high pulse of more than 2 microseconds
               //gives a short LOW pulse beforehand to ensure clean High signal
               pinMode(rPinWrite, OUTPUT);
               digitalWrite(rPinWrite, LOW);
               delayMicroseconds(2);
               digitalWrite(rPinWrite, HIGH);
               delayMicroseconds(5);
               digitalWrite(rPinWrite, LOW);
               
               //the same pin is used to read the signal from the ping
               //a high pulse whos duration is the time from the sending
               //of the ping to the reception of its echo off of an object
               pinMode(rPinRead, INPUT);
               duration = pulseIn(rPinRead, HIGH, 8000);
            break;
            case BACK:
              //ping is triggered by a high pulse of more than 2 microseconds
               //gives a short LOW pulse beforehand to ensure clean High signal
               pinMode(bPinWrite, OUTPUT);
               digitalWrite(bPinWrite, LOW);
               delayMicroseconds(2);
               digitalWrite(bPinWrite, HIGH);
               delayMicroseconds(5);
               digitalWrite(bPinWrite, LOW);
               
               //the same pin is used to read the signal from the ping
               //a high pulse whos duration is the time from the sending
               //of the ping to the reception of its echo off of an object
               pinMode(bPinRead, INPUT);
               duration = pulseIn(bPinRead, HIGH, 8000);
            break;
            case LEFT:
              //ping is triggered by a high pulse of more than 2 microseconds
               //gives a short LOW pulse beforehand to ensure clean High signal
               pinMode(lPinWrite, OUTPUT);
               digitalWrite(lPinWrite, LOW);
               delayMicroseconds(2);
               digitalWrite(lPinWrite, HIGH);
               delayMicroseconds(5);
               digitalWrite(lPinWrite, LOW);
               
               //the same pin is used to read the signal from the ping
               //a high pulse whos duration is the time from the sending
               //of the ping to the reception of its echo off of an object
               pinMode(lPinRead, INPUT);
               duration = pulseIn(lPinRead, HIGH, 8000);
            break;
         } 
         interrupts(); // This will enable the interrupts egain. DO NOT FORGET!
         
          //convert time to distance
           //feet = microsecondsToFeet(duration);
          // inches = microsecondsToInches(duration);
           cm = microsecondsToCentimeters(duration);
           return cm;
      }
      
      
      
      long microsecondsToInches(long microseconds){
       /*according to the datasheet there are 73.746 microseconds 
       per inch, 1130 ft per second. Divided by 2 for outbound and return
       */
       return microseconds / 74 / 2;
      }
      
      long microsecondsToFeet(long microseconds){
       /*according to the datasheet there are 73.746 microseconds 
       per inch, 1130 ft per second. Divided by 2 for outbound and return
       */
       return microsecondsToInches(microseconds)/12;
      }
      
      long microsecondsToCentimeters(long microseconds){
        /*The speed of sound is 340m/s or 29 microseconds
        per centimeter. divided by 2 for inbound and outboud trips.*/
        
        return microseconds / 29 / 2;
      }
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

void loop(){
  // Here is where the Threads are processed
  delay(500);
  Serial.print(" F: ");
  Serial.print(f);
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" L: ");
  Serial.println(l);
  //More Thread Processing done, why break it in half?
  delay(500);
}





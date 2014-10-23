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
    
    DistanceSensorThread(){
       Serial.println("constructor()");
    }
    
    //Used by the multi-threading library
    void run(){
      //Serial.println("run()");
      f = ping(0);
      r = ping(1);
      b = ping(2);
      l = ping(3); 
      runned();
    }
      
      float ping(int direction){
        float duration;
        long cm = 0;
        int readPin = 0;
        int writePin = 0;
        noInterrupts(); //disable multi-thread interrupts when pinging
        
          if(direction == 0){
            readPin = fPinRead;
            writePin = fPinWrite; 
          }else if(direction == 1){
            readPin = rPinRead;
            writePin = rPinWrite;
          }else if(direction == 2){
            readPin = bPinRead;
            writePin= bPinWrite; 
          }else if(direction == 3){
            readPin = lPinRead;
            writePin = lPinWrite; 
          }
         // Serial.print(" rpin: ");
          //Serial.println(readPin);
          //ping is triggered by a high pulse of more than 2 microseconds
               //gives a short LOW pulse beforehand to ensure clean High signal
               pinMode(writePin, OUTPUT);
               digitalWrite(writePin, LOW);
               delayMicroseconds(2);
               digitalWrite(writePin, HIGH);
               delayMicroseconds(5);
               digitalWrite(writePin, LOW);
               
               //the same pin is used to read the signal from the ping
               //a high pulse whos duration is the time from the sending
               //of the ping to the reception of its echo off of an object
               pinMode(readPin, INPUT);
               duration = pulseIn(readPin, HIGH, 8000);
          
        
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
    
};

//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
ThreadController controller = ThreadController();

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
  //Serial.println(" timerCallback() ");
  controller.run(); 
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
  distanceSensor.setInterval(100);//in milliseconds
  
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  
  //Initialize the timer used for multithreading.
  Timer1.initialize(50000); //in microseconds
  Timer1.attachInterrupt(timerCallback);
}

void loop(){
  // Here is where the Threads are processed
  delay(500);
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





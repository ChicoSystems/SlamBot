#include "Thread.h"
#include "ThreadController.h"
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

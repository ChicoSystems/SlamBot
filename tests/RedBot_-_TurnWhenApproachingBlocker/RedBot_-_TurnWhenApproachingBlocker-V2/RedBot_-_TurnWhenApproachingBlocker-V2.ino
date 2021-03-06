#include "RedBot.h"
#include <Wire.h>
#include "LSM303.h"
#include <math.h>

// Instantiate the motors.
RedBotMotor motors;

// Create an alias for our beeper pin, for ease of use.
#define BEEPER 9

LSM303 compass;

float pitch = 0; 
float roll = 0;
long l = 0, r = 0, f = 0, b = 0; 
const int FRONTp = 0;
const int RIGHTp = 1; 
const int BACKp = 2;
const int LEFTp = 3;
int goalHeading = 0;
int loopNum = 0;
float Heading = 0;
RedBotBumper bumper(3, &bump);
boolean bumped = false;

void setup(){
   // We *probably* won't see the hardware serial data; what good is a robot
  //  that has to be tethered to a computer?
  Serial.begin(9600);
  Wire.begin();  // Start up I2C, required for LSM303 communication
  compass.init();
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  compass.m_min = (LSM303::vector<int16_t>){-784, -623, -555};
  compass.m_max = (LSM303::vector<int16_t>){+322, +517, +489};
  
  Serial.println("TurnWhenApproachingBlocker V2");
  goalHeading = 45;
  delay(250);
}

void loop(){
  //Serial.println("LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP");
  tone(BEEPER, 150, 750);
  compass.read();
  Heading = compass.heading();
  
  Serial.print(" LOOP ");
  Serial.print(" HEADING: ");
  Serial.print(Heading);
  Serial.print(" Goal: ");
  Serial.print(goalHeading);
  Serial.println();
  turnTo(goalHeading, 0);
  moveUntil(25, 190, goalHeading);
  goalHeading = getLongestSide();
  loopNum++;
  delay(200);
}

void bump()
{
   Serial.println(" BUMPED: BUMPEDBUMPEDBUMPEDBUMPEDBUMPEDBUMPEDBUMPEDBUMPED");
  bumped = true;
  tone(BEEPER, 150, 750);
}

int getLongestSide(){
 // Serial.print(" getLongestSide ");
  compass.read();
  Heading = compass.heading();
  int longestSide = Heading;
  f = ping(FRONTp);
  r = ping(RIGHTp);
  b = ping(BACKp);
  l = ping(LEFTp);
  
    Serial.print("F: ");
  Serial.print(f);
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" L: ");
  Serial.println(l);
  
   if((f < 25 || f > 501) && f != 0){
     if(r < l){  
       if(l == 0){ // if l is zero we are probably timing out on l, so it's a greater distance, we want to turn that way.
          longestSide = Heading + 270;
       }else{
        longestSide = Heading + 90;
       }
     }else{
       if(r == 0){
          longestSide = Heading + 90;
       }else{
          longestSide = Heading + 270;
       }
     }
  }else{
        longestSide = Heading; 
     }
  /*
  if(f >= b && f >= l){
    longestSide = Heading; // the front is the longest
  }else if(l >= r){ //the left is biggest
    longestSide = Heading + 270;
  }else if(r > l){ //the right is biggest
    longestSide = Heading + 90;
  }*/
  
  longestSide = longestSide % 360;
  return longestSide;
}

/**
  moveUntil(int dist, float dir) will cause the bot to go forward in a compass corrected direction
  while measuring the front sonar. It will complete when the sonar senses an obstical within
  the dist limit - in centimeters. It will do it at the directed speed. 0-255
*/
void moveUntil(int dist, int speed, int dir){
  Serial.print(" moveUntil ");
  //slowSpeed calculates how fast the slower motor will spin while the bot is turning.
  int slowSpeed = (speed/2)+(speed/4)+(speed/8);
  
  /*acc will determine how many degree's accurate we want out movement to be.
    if it's too low the bot will stutter a lot while moving.*/
  int acc = 2;
  
  //We need the current measurement from the front distance sensor. This value will be re-read each time.
   f = ping(FRONTp);
   r = ping(RIGHTp);
   b = ping(BACKp);
   l = ping(LEFTp);
    Serial.print("F: ");
  Serial.print(f);
  Serial.print(" R: ");
  Serial.print(r);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" L: ");
  Serial.print(l);
    if((f < dist || f > 1000) && f != 0){
        for(int i = 0; i < 5; i++){
           f = ping(FRONTp); //getting some incorrect low pings, this will make us need to ping under the dist twice to trigger 
           if(f > dist) break;
           delay(15);
        }
     }
     if(f == 0){
        f = 10000; //over sensible distance fix
     }
  Serial.print(" P: ");
  Serial.print(f);
  while(f >= dist){
    compass.read();
    Heading = compass.heading();
    int t = getTurn(Heading, dir);
    if(abs(t) <= acc){ //go forward
      motors.drive(speed);
    }else if(t > 2){ //go slightly right
      motors.leftDrive(speed);
      motors.rightDrive(slowSpeed); 
    }else if(t < 2){ //go slightly left
      motors.leftDrive(slowSpeed);
      motors.rightDrive(speed); 
    }
    Serial.print(" P: ");
    Serial.print(f);
    Serial.print(" H :");
    Serial.print(Heading);
    Serial.print(" GH: ");
    Serial.print(dir);
    Serial.print(" T: ");
    Serial.print(t);
    Serial.print(" F: ");
    Serial.println(f);
    delay(20);
    f = ping(FRONTp);
     if((f < dist || f > 1000) && f != 0){
        for(int i = 0; i < 5; i++){
           f = ping(FRONTp); //getting some incorrect low pings, this will make us need to ping under the dist twice to trigger 
           if(f > dist) break;
        }
     }
     if(f == 0){
        f = 10000; //over sensible distance fix
     }
  }
  motors.stop();
}

//causes the redbot to go forward, while correcting it's orientation to match the goalHeading.
void forward(int speed, float goalHeading){
  int slowSpeed = (speed/2)+(speed/4)+(speed/8);
  while(true){
    compass.read();
    Heading = compass.heading();
    int t = getTurn(Heading, goalHeading);
    if(abs(t) <= 2){
      motors.drive(speed);
    }else if(t > 2){
      motors.leftDrive(speed);
      motors.rightDrive(slowSpeed); 
    }else if(t < 2){
       motors.leftDrive(slowSpeed);
       motors.rightDrive(speed); 
    }
    delay(50);
    Serial.print(" H :");
    Serial.print(Heading);
    Serial.print(" GH: ");
    Serial.print(goalHeading);
    Serial.print(" T: ");
    Serial.println(t);
  }
  motors.stop();
  delay(30);
}

void turnTo(float dir, int n){
  Serial.print(" turnTo ");
  if(n > 3) return;
  compass.read();
  Heading = compass.heading();
  int t = getTurn(Heading, dir);
  int mag; //how fast to turn based on how magnitude of turn
  int loopNum = 0; //lets us break out of while loop if motors are not working
  while(abs(t) >= 2 && loopNum < 100){
    if(n == 0){
      mag = map(abs(t), 0, 180, 180, 250);
    }else if(n == 1){
      mag = map(abs(t), 0, 180, 180, 220);
    }else if(n ==2){
      mag = map(abs(t), 0, 180, 170, 210);
    }else{
      mag = map(abs(t), 0, 180, 150, 200);
    }
    
    if( t >= 2){
       motors.pivot(-mag); 
    }else{
       motors.pivot(mag);
    }
    compass.read();
    Heading = compass.heading();
    t = getTurn(Heading, dir);
    n++;
    loopNum++;
  }
  motors.stop();
  delay(30); //compensate for problem with motor stop interfereing with compass.
  compass.read();
  Heading = compass.heading();
  t = getTurn(Heading, dir);
  if(abs(t) >= 2) turnTo(dir, n+1);
  Serial.print("°: ");
  Serial.print(Heading);
  Serial.print(" G°: ");
  Serial.println(dir);
}

/*
void turnTo(float dir, int numCalled){
  compass.read();
  Heading = compass.heading();
  int t = getTurn(Heading, dir);
  while(t >= 2){
    int mag = map(abs(t), 0, 180, 110, 250); //how fast to turn based on how magnitude of turn
    if(t >= 1){
      motors.pivot(-mag);
    }else{
      motors.pivot(mag);
    }
    delay(30);
    compass.read();
    Heading = compass.heading();
    t = getTurn(Heading, dir);
    Serial.print(numCalled);
     Serial.print(" Goal: ");
    Serial.print(dir);
    Serial.print(" HEADING: ");
    Serial.print(Heading);
    Serial.print(" t: ");
    Serial.print(t);
    Serial.print(" MAG:");
    Serial.println(mag);
  }
  motors.brake();
  int newNumCalled = numCalled + 1;
  if(abs(t) > 2) turnTo(dir, newNumCalled);
}

*/

int getTurn(float cur, float goal){
   int t = goal-cur;
    if(t > 180){
       t = 360 - t;
      t = -1 * t; 
    }else if(t < -180){
      t = t + 360;
      if(t < 0) t = -1 * t;
    } 
    return t;
}

long ping(int direction){
  long duration;
  long cm = 0;
   switch(direction){
      case FRONTp:
        //ping is triggered by a high pulse of more than 2 microseconds
         //gives a short LOW pulse beforehand to ensure clean High signal
         pinMode(A2, OUTPUT);
         digitalWrite(A2, LOW);
         delayMicroseconds(2);
         digitalWrite(A2, HIGH);
         delayMicroseconds(5);
         digitalWrite(A2, LOW);
         
         //the same pin is used to read the signal from the ping
         //a high pulse whos duration is the time from the sending
         //of the ping to the reception of its echo off of an object
         pinMode(A3, INPUT);
         duration = pulseIn(A3, HIGH, 8000);
      break;
      case RIGHTp:
        //ping is triggered by a high pulse of more than 2 microseconds
         //gives a short LOW pulse beforehand to ensure clean High signal
         pinMode(10, OUTPUT);
         digitalWrite(10, LOW);
         delayMicroseconds(2);
         digitalWrite(10, HIGH);
         delayMicroseconds(5);
         digitalWrite(10, LOW);
         
         //the same pin is used to read the signal from the ping
         //a high pulse whos duration is the time from the sending
         //of the ping to the reception of its echo off of an object
         pinMode(10, INPUT);
         duration = pulseIn(10, HIGH, 8000);
      break;
      case BACKp:
        //ping is triggered by a high pulse of more than 2 microseconds
         //gives a short LOW pulse beforehand to ensure clean High signal
         pinMode(11, OUTPUT);
         digitalWrite(11, LOW);
         delayMicroseconds(2);
         digitalWrite(11, HIGH);
         delayMicroseconds(5);
         digitalWrite(11, LOW);
         
         //the same pin is used to read the signal from the ping
         //a high pulse whos duration is the time from the sending
         //of the ping to the reception of its echo off of an object
         pinMode(11, INPUT);
         duration = pulseIn(11, HIGH, 8000);
      break;
      case LEFTp:
        //ping is triggered by a high pulse of more than 2 microseconds
         //gives a short LOW pulse beforehand to ensure clean High signal
         pinMode(A0, OUTPUT);
         digitalWrite(A0, LOW);
         delayMicroseconds(2);
         digitalWrite(A0, HIGH);
         delayMicroseconds(5);
         digitalWrite(A0, LOW);
         
         //the same pin is used to read the signal from the ping
         //a high pulse whos duration is the time from the sending
         //of the ping to the reception of its echo off of an object
         pinMode(A1, INPUT);
         duration = pulseIn(A1, HIGH, 8000);
      break;
   } 
   
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


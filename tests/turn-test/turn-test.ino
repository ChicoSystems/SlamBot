/*
 I'm using SparkFun's LSM303 breakout. Only power and the two
   I2C lines are connected:
   LSM303 Breakout ---------- Arduino
         Vin                   5V
         GND                   GND
         SDA                   A4
         SCL                   A5
*/

#include <RedBot.h>
#include <Wire.h>
#include <LSM303.h>
#include <math.h>

// Instantiate the motors.
RedBotMotor motors;

LSM303 compass;

float pitch = 0; 
float roll = 0;
long l = 0, r = 0, f = 0, b = 0; 
const int FRONT = 0;
const int RIGHT = 1; 
const int BACK = 2;
const int LEFT = 3;
int goalHeading = 0;
int loopNum = 0;
float Heading = 0;

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
  
  Serial.println("Turn-Test");
  goalHeading = 0;
  delay(250);
}

void loop(){
  Serial.println("LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP");
  compass.read();
  Heading = compass.heading();
  goalHeading = goalHeading + 90;
  goalHeading = goalHeading % 360;
  Serial.print(" Goal: ");
  Serial.print(goalHeading);
  Serial.print(" HEADING: ");
  Serial.print(Heading);
  Serial.println();
  turnTo(goalHeading, 0);
  loopNum++;
  delay(2500);
}

void turnTo(float dir, int n){
  if(n > 2) return;
  compass.read();
  Heading = compass.heading();
  int t = getTurn(Heading, dir);
  int mag; //how fast to turn based on how magnitude of turn
  while(abs(t) >= 2){
    if(n == 0){
      mag = map(abs(t), 0, 180, 150, 250);
    }else{
      mag = map(abs(t), 0, 180, 110, 150);
    }
    
    if( t >= 2){
       motors.pivot(-mag); 
    }else{
       motors.pivot(mag);
    }
    compass.read();
    Heading = compass.heading();
    t = getTurn(Heading, dir);
  }
  motors.stop();
  delay(10); //compensate for problem with motor stop interfereing with compass.
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
      case FRONT:
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
      case RIGHT:
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
      case BACK:
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
      case LEFT:
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






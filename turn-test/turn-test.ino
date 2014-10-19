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
  compass.read();
  //Heading = getCompassHeading();
  //goalHeading = goalHeading + 90;
  //goalHeading = goalHeading % 360;
  goalHeading = 180;
  Serial.print(" Goal: ");
  Serial.print(goalHeading);
  Serial.print(" HEADING: ");
  Serial.print(compass.heading());
  Serial.println();
  turnTo(goalHeading);
  loopNum++;
  delay(1500);
}

void turnTo(float dir){
  compass.read();
  Heading = compass.heading();
  while(abs(Heading - dir) >= 2){
    int a = dir-Heading;
    a = (a + 180) % 360 - 180; // decide which direction to turn
    int mag = map(abs(a), 0, 360, 100, 255); //how fast to turn based on how magnitude of turn
    Serial.print(" Goal: ");
    Serial.print(dir);
    Serial.print(" HEADING: ");
    Serial.print(Heading);
    Serial.print(" a: ");
    Serial.print(a);
    Serial.print(" MAG:");
    Serial.println(mag);
    if(a >= 1){
      motors.pivot(-mag);
    }else{
      motors.pivot(mag);
    }
    delay(1500);
    compass.read();
    Heading = compass.heading();
  }
  motors.brake();
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






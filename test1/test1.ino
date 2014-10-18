#include <RedBot.h>


// Instantiate the motors.
RedBotMotor motors;

// Instantiate the accelerometer. It can only be connected to pins A4
//  and A5, since those are the I2C pins for this board.
RedBotAccel xl;
int pingPin = 2;

void setup(){
   // We *probably* won't see the hardware serial data; what good is a robot
  //  that has to be tethered to a computer?
  Serial.begin(9600);
  Serial.println("Hello world!"); 
  delay(750);
}

void loop(){
   //variables to measure duration and distance of ping
  long duration, inches, cm, feet;
  
 //ping is triggered by a high pulse of more than 2 microseconds
 //gives a short LOW pulse beforehand to ensure clean High signal
 
 pinMode(pingPin, OUTPUT);
 digitalWrite(pingPin, LOW);
 delayMicroseconds(2);
 digitalWrite(pingPin, HIGH);
 delayMicroseconds(5);
 digitalWrite(pingPin, LOW);
 
 //the same pin is used to read the signal from the ping
 //a high pulse whos duration is the time from the sending
 //of the ping to the reception of its echo off of an object
 
 pinMode(pingPin, INPUT);
 duration = pulseIn(pingPin, HIGH);
 
 //convert time to distance
 feet = microsecondsToFeet(duration);
 inches = microsecondsToInches(duration);
 cm = microsecondsToCentimeters(duration);
 
  Serial.print(feet);
 Serial.print("ft, ");
 Serial.print(inches);
 Serial.print("in, ");
 Serial.print(cm);
 Serial.print("cm");
 Serial.println();
  
  /*
  motors.drive(180);
  delay(5000);
  motors.pivot(180);
  delay(2500);
  motors.pivot(-180);
  delay(2500);
  motors.brake();
  delay(500);
  */
}

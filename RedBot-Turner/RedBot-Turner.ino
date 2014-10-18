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
#include <math.h>

#define SCALE 2  // accel full-scale, should be 2, 4, or 8

/* LSM303 Address definitions */
#define LSM303_MAG  0x1E  // assuming SA0 grounded
#define LSM303_ACC  0x18  // assuming SA0 grounded

#define X 0
#define Y 1
#define Z 2

/* LSM303 Register definitions */
#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define HP_FILTER_RESET_A 0x25
#define REFERENCE_A 0x26
#define STATUS_REG_A 0x27
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define INT1_CFG_A 0x30
#define INT1_SOURCE_A 0x31
#define INT1_THS_A 0x32
#define INT1_DURATION_A 0x33
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Y_H_M 0x05
#define OUT_Y_L_M 0x06
#define OUT_Z_H_M 0x07
#define OUT_Z_L_M 0x08
#define SR_REG_M 0x09
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C

/* Global variables */
int accel[3];  // we'll store the raw acceleration values here
int mag[3];  // raw magnetometer values stored here
float realAccel[3];  // calculated acceleration values here

// Instantiate the motors.
RedBotMotor motors;

float pitch = 0; 
float roll = 0;
long l = 0, r = 0, f = 0, b = 0, heading = 0;
const int FRONT = 0;
const int RIGHT = 1; 
const int BACK = 2;
const int LEFT = 3;
int goalHeading = 0;
void setup(){
   // We *probably* won't see the hardware serial data; what good is a robot
  //  that has to be tethered to a computer?
  Serial.begin(9600);
  Wire.begin();  // Start up I2C, required for LSM303 communication
  initLSM303(SCALE);  // Initialize the LSM303, using a SCALE full-scale range
  
  Serial.println("MultiSensorTest"); 
  heading = getCompassHeading();
  //goalHeading = heading + 90;
  //goalHeading = goalHeading % 360;
  goalHeading = 180;
  delay(250);
}

void loop(){
  heading = getCompassHeading();
  
  //
  Serial.print("Heading: ");
  Serial.print(heading);
  Serial.print(" Goal: ");
  Serial.print(goalHeading);
  Serial.println();
  
  if(heading > goalHeading + 90){
    motors.pivot(-120);
  }else if(heading < goalHeading - 90){
    motors.pivot(120); 
  }else{
    motors.brake(); 
  }
  

 
  //Serial.println("f r b l: " + f + " " + r + " " + b + " " + l + " H: " + heading);
  delay(150);
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
         duration = pulseIn(A3, HIGH);
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
         duration = pulseIn(10, HIGH);
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
         duration = pulseIn(11, HIGH);
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
         duration = pulseIn(A1, HIGH);
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

long getCompassHeading(){
   getLSM303_accel(accel);  // get the acceleration values and store them in the accel array
    while(!(LSM303_read(SR_REG_M) & 0x01))
      ;  // wait for the magnetometer readings to be ready
    getLSM303_mag(mag);  // get the magnetometer values, store them in mag
    //printValues(mag, accel);  // print the raw accel and mag values, good debugging
    
    for (int i=0; i<3; i++)
      realAccel[i] = accel[i] / pow(2, 15) * SCALE;  // calculate real acceleration values, in units of g
    
    return getTiltHeading(mag, realAccel);
}

void initLSM303(int fs)
{
  LSM303_write(0x27, CTRL_REG1_A);  // 0x27 = normal power mode, all accel axes on
  if ((fs==8)||(fs==4))
    LSM303_write((0x00 | (fs-fs/2-1)<<4), CTRL_REG4_A);  // set full-scale
  else
    LSM303_write(0x00, CTRL_REG4_A);
  LSM303_write(0x14, CRA_REG_M);  // 0x14 = mag 30Hz output rate
  LSM303_write(0x00, MR_REG_M);  // 0x00 = continouous conversion mode
}

void printValues(int * magArray, int * accelArray)
{
  /* print out mag and accel arrays all pretty-like */
  Serial.print(accelArray[X], DEC);
  Serial.print("\t");
  Serial.print(accelArray[Y], DEC);
  Serial.print("\t");
  Serial.print(accelArray[Z], DEC);
  Serial.print("\t\t");
  
  Serial.print(magArray[X], DEC);
  Serial.print("\t");
  Serial.print(magArray[Y], DEC);
  Serial.print("\t");
  Serial.print(magArray[Z], DEC);
  Serial.println();
}

float getHeading(int * magValue)
{
  // see section 1.2 in app note AN3192
  float heading = 180*atan2(magValue[Y], magValue[X])/PI;  // assume pitch, roll are 0
  
  if (heading <0)
    heading += 360;
  
  return heading;
}

float getTiltHeading(int * magValue, float * accelValue)
{
  // see appendix A in app note AN3192 
  pitch = asin(-accelValue[X]);
  roll = asin(accelValue[Y]/cos(pitch));
  
  float xh = magValue[X] * cos(pitch) + magValue[Z] * sin(pitch);
  float yh = magValue[X] * sin(roll) * sin(pitch) + magValue[Y] * cos(roll) - magValue[Z] * sin(roll) * cos(pitch);
  float zh = -magValue[X] * cos(roll) * sin(pitch) + magValue[Y] * sin(roll) + magValue[Z] * cos(roll) * cos(pitch);

  float heading = 180 * atan2(yh, xh)/PI;
  if (yh >= 0)
    return heading;
  else
    return (360 + heading);
}

void getLSM303_mag(int * rawValues)
{
  Wire.beginTransmission(LSM303_MAG);
  Wire.write(OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom(LSM303_MAG, 6);
  for (int i=0; i<3; i++)
    rawValues[i] = (Wire.read() << 8) | Wire.read();
}

void getLSM303_accel(int * rawValues)
{
  rawValues[Z] = ((int)LSM303_read(OUT_X_L_A) << 8) | (LSM303_read(OUT_X_H_A));
  rawValues[X] = ((int)LSM303_read(OUT_Y_L_A) << 8) | (LSM303_read(OUT_Y_H_A));
  rawValues[Y] = ((int)LSM303_read(OUT_Z_L_A) << 8) | (LSM303_read(OUT_Z_H_A));  
  // had to swap those to right the data with the proper axis
}

byte LSM303_read(byte address)
{
  byte temp;
  
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
    
  Wire.write(address);
  
  if (address >= 0x20)
    Wire.requestFrom(LSM303_ACC, 1);
  else
    Wire.requestFrom(LSM303_MAG, 1);
  while(!Wire.available())
    ;
  temp = Wire.read();
  Wire.endTransmission();
  
  return temp;
}

void LSM303_write(byte data, byte address)
{
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
    
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

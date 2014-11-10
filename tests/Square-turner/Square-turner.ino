//** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include "DistanceSensorThread.h"
#include "TimerOne.h"
#include "RedBot.h"
#include "CompassThread.h"
//#include "SerialCommand.h"
#include <Wire.h>

// Instantiate the motors.
RedBotMotor motors;

// Instantiate our encoder. 
RedBotEncoder encoder = RedBotEncoder(11, 10); // left, right

//Initiate our distance sensor and the controller
//DistanceSensorThread distanceSensor = DistanceSensorThread();
CompassThread compass = CompassThread(); //we're not using this as a thread here
//ThreadController controller = ThreadController();

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
 // Serial.print(" timerCallback() ");
  //controller.run(); 
}

void setup(){
  Serial.begin(9600); 
  Serial.println("Square-turner");
  compass.init();
  
  //distanceSensor.fPinWrite = A2;
  //distanceSensor.fPinRead = A3;
  //distanceSensor.rPinWrite = 3; 
  //distanceSensor.rPinRead = 3;
  //distanceSensor.bPinWrite = 9;
  //distanceSensor.bPinRead = 9;
  //distanceSensor.lPinWrite = A0;
  //distanceSensor.lPinRead = A1;
  
  //set the interval that the clterval that tass will run at.
  //distanceSensor.setInterval(100);//in milliseconds
  //compass.setInterval(100);
  //controller.add(&distanceSensor);
  // Timer1.initialize(100000); //in microseconds
  //Timer1.attachInterrupt(timerCallback);
  
  //encoders don't work unless we power motors first.
  motors.brake();
  motors.drive(255);
  delay(50);
  motors.stop();
        
  encoder.clearEnc(BOTH);
  //goalHeading = compass.getHeading();
  
}

void loop(){
  delay(50);
  float goal = 100;
  turn(100);
  delay(10000);
  turn(180);
  delay(10000);
  
  
}

/**
  * Moves the bot in the specified direction, the specified distance, or 
  * until the front sensor senses something blocking it at blocked_dist
  * the bot will move at the speed directed 0-255
  */
void move(int dir, int dist_ticks, int blocked_dist_cm, int speed){
  Serial.print("move()- dir:");
  Serial.print(dir);
  Serial.print(" dist_ticks:");
  Serial.print(dist_ticks);
  Serial.print(" blocked_dist_cm:");
  Serial.print(blocked_dist_cm);
  Serial.print(" speed:");
  Serial.print(speed);
  
  //slowSpeed calculates how fast the slower motor will spin while the bot is turning.
  int slowSpeed = (speed/2)+(speed/4)+(speed/8);
  
  /*acc will determine how many degree's accurate we want out movement to be.
    if it's too low the bot will stutter a lot while moving.*/
  int acc = 2;
  
  // Used to calculate the average number of ticks from each motor since the start
  // of this function.
  int avg_ticks_from_start = 0;
  
  /* encoderStart will be used to help keep track of how many encoder
     ticks each wheel has done since the function started. */
  int encoderStart [] = {0,1};
  encoderStart[0] = encoder.getTicks(LEFT);
  encoderStart[1] = encoder.getTicks(RIGHT);
  
  boolean keepMoving = true; // keeps track if we should keep moving or not.
  //if(distanceSensor.f <= blocked_dist_cm) keepMoving = false;
  
  while(keepMoving){
    delay(10);
    // calculate the degree's we need to turn to reach dir goal
    int t = getTurn(compass.getHeading(), dir); 
    
    //go forward if t is small enough, otherwise turn the appropriate direction.
    if(abs(t) <= acc){ //go forward
      motors.drive(speed);
    }else if(t > 2){ //go slightly right
      motors.leftDrive(speed);
      motors.rightDrive(slowSpeed); 
    }else if(t < 2){ //go slightly left
      motors.leftDrive(slowSpeed);
      motors.rightDrive(speed); 
    }
    
    //calculates the average amount of ticks between both engines since the function started
    avg_ticks_from_start = ((encoder.getTicks(LEFT)-encoderStart[0]) 
                                + (encoder.getTicks(RIGHT)-encoderStart[1]) / 2);
                     
    Serial.print("currentHeading:");
    Serial.print(compass.getHeading());           
    Serial.print(" Avg Ticks:");
    Serial.print(avg_ticks_from_start);
    //Serial.print(" f-ping:");
    //Serial.println(distanceSensor.f);
    
    // If ping is too small, and not 0, or if our avg ticks are to big we will stop moving
    if(((/*distanceSensor.f < blocked_dist_cm && distanceSensor.f != 0 */false) 
        || avg_ticks_from_start >= dist_ticks)) {
        keepMoving = false;
        }
    //if(distanceSensor.f <= blocked_dist_cm && distanceSensor.f != 0){
    //   keepMoving = false
    // }
        
    
  }
  motors.stop();
  Serial.print("DONE: move()- currentHeading:");
  Serial.print(compass.getHeading());
 // Serial.print(" f-ping:");
  //Serial.print(distanceSensor.f);
  Serial.print(" avg_ticks_from_start:");
  Serial.println(avg_ticks_from_start);
}

// calculates the correct turn to make to reach the goal from the current rotation
float getTurn(float cur, float goal){
   float t = goal-cur;
    if(t > 180){
       t = 360 - t;
      t = -1 * t; 
    }else if(t < -180){
      t = t + 360;
      if(t < 0) t = -1 * t;
    } 
    return t;
}

void turn(float goal){
  float heading = compass.getHeading();
  float t = getTurn(heading, goal);
  float mag = map(abs(t), 0, 180, 150, 200);
  while(abs(t) > 5){
    Serial.print(" Goal: ");
    Serial.print(goal);
    Serial.print(" Heading: ");
    Serial.print(heading);
    Serial.print(" T: ");
    Serial.print(t);
    Serial.print(" Mag: ");
    Serial.println(mag);
    if( t > 5){
       motors.pivot(mag); 
    }else if(t < 5){
       motors.pivot(-mag);
    }else{
       motors.stop(); 
    }
    
    heading = compass.getHeading();
    t = getTurn(heading, goal);
    mag = map(abs(t), 0, 180, 100, 150);
    delay(20);
  }
}

void turnTo(float dir, int n){
  float heading;
  Serial.println(" turnTo: ");
  if(n > 3) return;
  heading = compass.getHeading();
  int t = getTurn(heading, dir);
  int mag; //how fast to turn based on how magnitude of turn
  int loopNum = 0; //lets us break out of while loop if motors are not working
  while(abs(t) > 5 && loopNum < 10){
  
    if(n == 0){
      mag = map(abs(t), 0, 180, 160, 200);
    }else if(n == 1){
      mag = map(abs(t), 0, 180, 150, 190);
    }else if(n == 2){
      mag = map(abs(t), 0, 180, 140, 180);
    }else{
      mag = map(abs(t), 0, 180, 140, 170);
    }
  Serial.print(" N: ");
  Serial.print(n);
  Serial.print(" T: ");
  Serial.print(t);
  Serial.print(" MAG: ");
  Serial.print(mag);
  Serial.print(" CurrentHeading ");
  Serial.print(heading);
  Serial.print(" GoalHeading: ");
  Serial.println(dir);
    if( t >= 2){
       motors.pivot(mag); 
    }else{
       motors.pivot(-mag);
    }
    heading = compass.getHeading();
    t = getTurn(heading, dir);
    loopNum++;
  }
  motors.stop();
  delay(50); //compensate for problem with motor stop interfereing with compass.
  heading = compass.getHeading();
  t = getTurn(heading, dir);
  if(abs(t) > 5){
    delay(1000);
    turnTo(dir, n+1);
  }
  Serial.print("TurnToDone h:");
  Serial.print(heading);
  Serial.print(" Goal: ");
  Serial.print(dir);
  Serial.print(" N: ");
  Serial.println(n);
 
}

//** Experiementing with multithreading library ArduinoThread */
#include "Thread.h"
#include "ThreadController.h"
#include "DistanceSensorThread.h"
#include "TimerOne.h"
#include "RedBot.h"
#include "CompassThread.h"
#include "SerialCommand.h"



#include <Wire.h>

// Create an alias for our pins
#define BEEPER A7
#define BUMPER A6
#define arduinoLED 13   // Arduino LED on board

// Instantiate the motors.
RedBotMotor motors;

// Instantiate our encoder. 
RedBotEncoder encoder = RedBotEncoder(11, 10); // left, right

//Initiate our distance sensor and the controller
DistanceSensorThread distanceSensor = DistanceSensorThread();
CompassThread compass = CompassThread(); //we're not using this as a thread here
ThreadController controller = ThreadController();
RedBotBumper lBumper(BUMPER, &bump);
SerialCommand sCmd;     // The demo SerialCommand object

float goalHeading = 0;
boolean bumped = false;

//The function that the timer will call for the DistanceSensor.
void timerCallback(){
 // Serial.print(" timerCallback() ");
  controller.run(); 
}

void setup(){
  Serial.begin(9600); 
  Serial.println("SLAMBOT");
  
  tone(BEEPER, 2600, 750);
  
  // Send out a hello via the XBee, if one is present.
 // swsp.begin(9600);
  //swsp.print("Hello, world");
  Serial.println("initializing compass");
  compass.init();
  Serial.println("compajss initialized");
  //setup pins to distanceSensor
  distanceSensor.fPinWrite = A2;
  distanceSensor.fPinRead = A3;
  distanceSensor.rPinWrite = 3; 
  distanceSensor.rPinRead = 3;
  distanceSensor.bPinWrite = 9;
  distanceSensor.bPinRead = 9;
  distanceSensor.lPinWrite = A0;
  distanceSensor.lPinRead = A1;
  
  //set the interval that the clterval that tass will run at.
  distanceSensor.setInterval(100);//in milliseconds
  compass.setInterval(100);
  
  Serial.println("adding distance sensor");
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  controller.add(&compass);
  Serial.println("initializing timer");
  //Initialize the timer used for multithreading.
  Timer1.initialize(100000); //in microseconds
  Timer1.attachInterrupt(timerCallback);
  Serial.println("timer initialized");
  
  //encoders don't work unless we power motors first.
  motors.brake();
        motors.drive(255);
        delay(50);
        motors.stop();
  
  encoder.clearEnc(BOTH);
  goalHeading = compass.getHeading();
  Serial.println(goalHeading);
  
  //add commands to allow remote control over serial
  sCmd.addCommand("ON",    LED_on);          // Turns LED on
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("T", processTurnCommand);
  sCmd.addCommand("M", processMoveCommand);
  sCmd.setDefaultHandler(processUnrecognizedCommand);      // Handler for command that isn't matched  (says "What?")
}

void loop(){
  //swsp.println(distanceSensor.f);
  // Here is where the Threads are processed
  delay(500);
  // move(goalHeading, 500, 20, 180);
  sCmd.readSerial();
 // Serial.println(compass.heading);
  
  if(bumped)bumped = !bumped;
  //More Thread Processing done, why break it in half?
  delay(500);
}

void bump(){
  bumped = true;
  tone(BEEPER, 150, 750);
}

// This gets set as the default handler, and gets called when no other command matches.
void processUnrecognizedCommand(const char *command) {
  Serial.println("What?");
}

/* Processes the turn command coming in over serial. */
void processTurnCommand() {
  int aNumber;
  char *arg;

  Serial.println("We're in processTurnCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
    turnTo(aNumber, 0);
  }else {
    Serial.println("No arguments");
  }
}

/* Processes the turn command coming in over serial. */
void processMoveCommand() {
  int dir, ticks, blocked_dist_cm, sp;
  
  int aNumber;
  char *arg;

  Serial.println("We're in processMoveCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
    dir = aNumber;
    arg = sCmd.next();
    if (arg != NULL) {
      aNumber = atoi(arg);    // Converts a char string to an integer
      Serial.print("2nd argument was: ");
      Serial.println(aNumber);
      ticks = aNumber;
      arg = sCmd.next();
      if (arg != NULL) {
        aNumber = atoi(arg);    // Converts a char string to an integer
        Serial.print("3rd argument was: ");
        Serial.println(aNumber);
        blocked_dist_cm = aNumber;
        arg = sCmd.next();
        if (arg != NULL) {
          aNumber = atoi(arg);    // Converts a char string to an integer
          Serial.print("4th argument was: ");
          Serial.println(aNumber);
          sp = aNumber;
        }else{
          Serial.println("No 4th Argument");
        }
      }else{
        Serial.println("No 3nd Argument");
      }
    }else{
      Serial.println("No 2nd Argument");
    }
  }else {
    Serial.println("No First Argument");
  }
  move(dir, ticks, blocked_dist_cm, sp);
}

void LED_on() {
  Serial.println("LED on");
  digitalWrite(arduinoLED, HIGH);
}

void LED_off() {
  Serial.println("LED off");
  digitalWrite(arduinoLED, LOW);
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
  if(distanceSensor.f <= blocked_dist_cm) keepMoving = false;
  
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
    Serial.print(" f-ping:");
    Serial.println(distanceSensor.f);
    
    // If ping is too small, and not 0, or if our avg ticks are to big we will stop moving
    if(((distanceSensor.f < blocked_dist_cm && distanceSensor.f != 0) 
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
  Serial.print(" f-ping:");
  Serial.print(distanceSensor.f);
  Serial.print(" avg_ticks_from_start:");
  Serial.println(avg_ticks_from_start);
}

// calculates the correct turn to make to reach the goal from the current rotation
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

void turnTo(float dir, int n){
  Serial.print(" turnTo: ");
  if(n > 3) return;
  int t = getTurn(compass.getHeading(), dir);
  int mag; //how fast to turn based on how magnitude of turn
  int loopNum = 0; //lets us break out of while loop if motors are not working
  while(abs(t) >= 2 && loopNum < 100){
      Serial.print(" N: ");
  Serial.print(n);
 Serial.print(" T: ");
  Serial.print(t);
  Serial.print(" CurrentHeading ");
  Serial.print(compass.getHeading());
  Serial.print(" GoalHeading: ");
  Serial.println(dir);
    if(n == 0){
      mag = map(abs(t), 0, 180, 180, 250);
    }else if(n == 1){
      mag = map(abs(t), 0, 180, 180, 220);
    }else if(n == 2){
      mag = map(abs(t), 0, 180, 170, 210);
    }else{
      mag = map(abs(t), 0, 180, 150, 200);
    }
    
    if( t >= 2){
       motors.pivot(-mag); 
    }else{
       motors.pivot(mag);
    }
    t = getTurn(compass.getHeading(), dir);
    n++;
    loopNum++;
  }
  motors.stop();
  delay(30); //compensate for problem with motor stop interfereing with compass.
  t = getTurn(compass.getHeading(), dir);
  if(abs(t) >= 2) turnTo(dir, n+1);
 
}




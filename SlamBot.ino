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
#define DEBUG

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
  debug("SLAMBOT", true);
  
  tone(BEEPER, 2600, 750);
  
  // Send out a hello via the XBee, if one is present.
 // swsp.begin(9600);
  //swsp.print("Hello, world");
  debug(" initializing compass ", 1);
  compass.init();
  debug(" compajss initialized ");
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
  
  debug(" adding distance sensor ");
  //add the DistanceSensor to the Thread Controller using it's pointer
  controller.add(&distanceSensor);
  controller.add(&compass);
  debug(" initializing timer" );
  //Initialize the timer used for multithreading.
  Timer1.initialize(100000); //in microseconds
  Timer1.attachInterrupt(timerCallback);
  debug(" timer initialized ", 1);
  
  //encoders don't work unless we power motors first.
  motors.brake();
        motors.drive(255);
        delay(50);
        motors.stop();
  
  encoder.clearEnc(BOTH);
  goalHeading = compass.getHeading();
  debug(goalHeading);
  
  //add commands to allow remote control over serial
  sCmd.addCommand("ON",    LED_on);          // Turns LED on
  sCmd.addCommand("OFF",   LED_off);         // Turns LED off
  sCmd.addCommand("T", processTurnCommand);
  sCmd.addCommand("M", processMoveCommand);
  sCmd.addCommand("H", processHeadingCommand);
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

void processHeadingCommand() {
  Serial.println(compass.getHeading());
}

/* Processes the turn command coming in over serial. */
void processTurnCommand() {
  int aNumber;
  char *arg;

  debug(" We're in processTurnCommand ", 1);
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    debug(" First argument was: ");
    debug(aNumber, 1);
    turnTo(aNumber, 0);
  }else {
    debug("No arguments", 1);
  }
}

/* Processes the turn command coming in over serial. */
void processMoveCommand() {
  int dir, ticks, blocked_dist_cm, sp;
  
  int aNumber;
  char *arg;

  debug("We're in processMoveCommand", 1);
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    debug("First argument was: ");
    debug(aNumber, 1);
    dir = aNumber;
    arg = sCmd.next();
    if (arg != NULL) {
      aNumber = atoi(arg);    // Converts a char string to an integer
      debug("2nd argument was: ");
      debug(aNumber, 1);
      ticks = aNumber;
      arg = sCmd.next();
      if (arg != NULL) {
        aNumber = atoi(arg);    // Converts a char string to an integer
        debug("3rd argument was: ");
        debug(aNumber, 1);
        blocked_dist_cm = aNumber;
        arg = sCmd.next();
        if (arg != NULL) {
          aNumber = atoi(arg);    // Converts a char string to an integer
          debug("4th argument was: ");
          debug(aNumber, 1);
          sp = aNumber;
        }else{
          debug("No 4th Argument", 1);
        }
      }else{
        debug("No 3nd Argument", 1);
      }
    }else{
      debug("No 2nd Argument", 1);
    }
  }else {
    debug("No First Argument", 1);
  }
  move(dir, ticks, blocked_dist_cm, sp);
}

void LED_on() {
  debug("LED on", 1);
  digitalWrite(arduinoLED, HIGH);
}

void LED_off() {
  debug("LED off", 1);
  digitalWrite(arduinoLED, LOW);
}

/**
  * Moves the bot in the specified direction, the specified distance, or 
  * until the front sensor senses something blocking it at blocked_dist
  * the bot will move at the speed directed 0-255
  */
void move(int dir, int dist_ticks, int blocked_dist_cm, int speed){
  debug("move()- dir:");
  debug(dir);
  debug(" dist_ticks:");
  debug(dist_ticks);
  debug(" blocked_dist_cm:");
  debug(blocked_dist_cm);
  debug(" speed:");
  debug(speed, 1);
  
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
                     
    debug("currentHeading:");
    debug(compass.getHeading());           
    debug(" Avg Ticks:");
    debug(avg_ticks_from_start);
    debug(" f-ping:");
    debug(distanceSensor.f, 1);
    
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
  debug("DONE: move()- currentHeading:");
  debug(compass.getHeading());
  debug(" f-ping:");
  debug(distanceSensor.f);
  debug(" avg_ticks_from_start:");
  debug(avg_ticks_from_start, 1);
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
  float heading;
  debug(" turnTo: ", 1);
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
  debug(" N: ");
  debug(n);
  debug(" T: ");
  debug(t);
  debug(" MAG: ");
  debug(mag);
  debug(" CurrentHeading ");
  debug(heading);
  debug(" GoalHeading: ");
  debug(dir, 1);
    if( t >= 2){
       motors.pivot(-mag); 
    }else{
       motors.pivot(mag);
    }
    heading = compass.getHeading();
    t = getTurn(heading, dir);
    loopNum++;
  }
  motors.stop();
  delay(30); //compensate for problem with motor stop interfereing with compass.
  heading = compass.getHeading();
  t = getTurn(heading, dir);
  if(abs(t) > 5) turnTo(dir, n+1);
  debug("TurnToDone h:");
  debug(heading);
  debug(" Goal: ");
  debug(dir);
  debug(" N: ");
  debug(n);
 
}

/** Used to display debug values to Serial. */
void debug(String mesg, bool newLine = false){
  #ifdef DEBUG
    if(newLine){
       Serial.println(mesg);
    }else{
       Serial.print(mesg); 
    }
  #endif
}

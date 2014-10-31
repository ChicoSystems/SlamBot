// Include the library.
#include "RedBot.h"

// Instantiate the motors.
RedBotMotor motors;

// Instantiate our encoder. 
RedBotEncoder encoder = RedBotEncoder(11, 10); // left, right


// Instantiate a couple of whisker switches. Call bump() when one of them
//  hits something. There's no stopping you having a different function for
//  each whisker; I just chose not to.
RedBotBumper lBumper(3, &bump);

// Instantiate a software serial port. Note that the regular Arduino
//  SoftwareSerial won't work! It steals resources that the sensor
//  inputs on the RedBot need. Also note that the RedBot version has
//  no input options- it automatically connects to the pins used for
//  the onboard XBee header.
//RedBotSoftwareSerial swsp;


// Create an alias for our beeper pin, for ease of use.
#define BEEPER 9

// Create an alias for the onboard pushbutton.
#define BUTTON_PIN 12

// Create an alias for the onboard LED.
#define LED_PIN 13

// Constants for the levels that determine detection of the line.
const int bgLevel = 600;
const int lineLevel = 700;

// Flag for bumpers to send the signal that something's wrong and the motors should
//  be disabled for now. Set in the bump functions, volatile so the change is sure
//  to propagate to the loop() function.
volatile boolean bumped = true;

void setup()
{
  // We *probably* won't see the hardware serial data; what good is a robot
  //  that has to be tethered to a computer?
  Serial.begin(9600);
  Serial.println("Hello world!");
  // Set up our two built-in IO devices- the button and the LED.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  // Play a tone for audible reset detection.
  tone(BEEPER, 2600, 750);
  delay(750);
  // Send out a hello via the XBee, if one is present.
  //swsp.begin(9600);
  Serial.print("Hello, world");
}

void loop()
{
  // These static variables will be initialized once, then remain valid
  //  through each iteration of the loop.

  // Two loop timers; we have one loop that happens four times a second
  //  and one that happens ten times a second.
  static unsigned long loopStart = millis();
  static unsigned long loopStart2 = millis();
  // We're doing a crude low-pass filter on the accelerometer; we want
  //  two consecutive high values so we don't trigger our surprise on
  //  transient events, only on a significant change in angle.

  // These two variables handle the current position and position
  //  change of the scanning servo motor.
  static int angle = 0;
  static int angleDelta = 5;

  // Wait for the button to be pressed, then turn off the "bumped" flag
  //  so the motors can run. Also, clear the encoders, so we can track
  //  our motion.
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    bumped = false;
    encoder.clearEnc(BOTH);
  }
  // Dump the current distance count on the right wheel to the serial
  //  port, just for something to do.
  Serial.println(encoder.getTicks(RIGHT));
  // If we pass 75 ticks on the right wheel, treat that the same as we
  //  would a bumper hit and stop moving.
  if (encoder.getTicks(RIGHT) > 75) bumped = true;

  // This is our ten-times-a-second loop. It's watching the accelerometer
  //  and, when the bot senses an incline, setting off our surprise.
  if (loopStart2 + 100 < millis())
  {
    loopStart2 = millis();
 
  
    if (!bumped)
    {
        motors.brake();
        horn();
        motors.drive(255);
        delay(5000);
        motors.brake();
        bumped = true; 
      
    }
  }

  // All this line-follow-y stuff should only be invoked when we are clear to move;
  //  if one of the bump sensors was set off, we should stay still until the user
  //  intervenes for us.
  if (!bumped)
  {
      motors.leftDrive(-100);
      motors.rightDrive(-100);
  }
  else motors.brake();
}

// This is the function that gets called when we bump something. It
//  stops the motors, signals that a bump occurred (so loop() doesn't
//  just start the motors back up), and issues a nasty little tone to
//  tell the user what's up.
void bump()
{
  motors.brake();
  bumped = true;
  tone(BEEPER, 150, 750);
}

// This is all for our surprise. SN is the length of a sixteenth note,
//  in milliseconds, and EN is the length of an eighth note.
const int SN = 100;
const int EN = 200;
void horn()
{
  
}

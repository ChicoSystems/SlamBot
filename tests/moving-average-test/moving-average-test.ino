#include <Wire.h>
#include "CompassThread.h"

CompassThread compass = CompassThread(); //we're not using this as a thread here

void setup(){
  Serial.begin(9600); 
  compass.init();
}

void loop(){
  //Serial.println("Compass: ");
  //Serial.println(compass.getHeading());
  compass.getHeading();
  delay(1000);
}

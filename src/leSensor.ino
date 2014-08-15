/*

  leSensor.cpp

  Copyright (c) 2014, Mauricio Bustos
  All rights reserved.

  Redistribution and use in source and binary forms, 
  with or without modification, are permitted provided 
  that the following conditions are met:

  - Redistributions must not be for profit.  This software in source or binary form, 
  with or without modification, cannot be included in anyproduct that is then sold.  
  Please contact Mauricio Bustos m@bustos.org if you are interested in a for-profit license.
  - Redistributions of source code or in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Arduino.h"
#include "SerialCommand.h"

// Available Touch Pins:
//  15, 16, 17, 18, 19, 22, 23

long lastTouchRead[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long lastTouchMin[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t touchPins[] = {15, 16, 17, 18, 19, 22, 23};
unsigned long lastTouchTimeStamp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define TOUCH_PIN_COUNT (7)
long touchThreshold = 1000;
bool testing = false;
int filterLength = 2;
int throttle = 1000;
bool sendUpdate = false;
unsigned long lastTransmitTime = 0;
const int baudRate = 57600;

SerialCommand SCmd;

// Prototypes
bool detectTouch (int touchMe);
void sendSensorData();
void enableTestMode();
void disableTestMode();
void processThresholdRequest();
void processFilterLengthRequest();
void processThrottleRequest();
void processSettingsRequest();
void unrecognized();

//! Setup system
void setup()
{
  SCmd.addCommand("TEST", enableTestMode);
  SCmd.addCommand("PROD", disableTestMode);
  SCmd.addCommand("THRS", processThresholdRequest);
  SCmd.addCommand("FILT", processFilterLengthRequest);
  SCmd.addCommand("THRT", processThrottleRequest);
  SCmd.addCommand("SETT", processSettingsRequest);
  SCmd.addDefaultHandler(unrecognized);
  pinMode(13, OUTPUT);
  Serial.begin(baudRate);
}

//! Main loop
void loop()
{
  unsigned long timestamp = millis();
  if (timestamp % 1000 < 50) {
	digitalWrite(13, HIGH);
  } else {
	digitalWrite(13, LOW);
  }
  // Check to see if a command has been issued to us
  SCmd.readSerial();
  if (testing) {
	delay (1000);
	sendSensorData();
  } else {
	// Read sensors and update if appropriate
	for (uint8_t i = 0; i < TOUCH_PIN_COUNT; i++) {
	  if (detectTouch (i)) {
		sendUpdate = true;
		lastTouchTimeStamp [i] = millis();
	  } else if (lastTouchTimeStamp [i] > 0 && millis() - lastTouchTimeStamp [i] > 5000) {
		lastTouchTimeStamp [i] = 0;
		if (abs (lastTouchRead [i] - lastTouchMin [i]) > 10) sendUpdate = true;
		lastTouchRead [i] = lastTouchMin [i];
	  }
	}
	if (sendUpdate && (millis() > lastTransmitTime + throttle)) {
	  lastTransmitTime = millis();
	  sendUpdate = false;
	  sendSensorData();
	}
  }
  delay (10);
}

//! Send out our current sensor readings
void sendSensorData() {
  Serial.print ("DATA:");
  for (uint8_t i = 0; i < TOUCH_PIN_COUNT; i++) {
	Serial.print("s_");
	Serial.print(i);
	Serial.print("=");
	Serial.print(lastTouchRead[i]);
	if (i < TOUCH_PIN_COUNT - 1) {
	  Serial.print(",");
	}
  }
  Serial.println("");
}

//! Have we detected a touch on pin `touchMe'?
bool detectTouch (int touchMe) {
  long touch = touchRead(touchPins[touchMe]);
  if (lastTouchRead [touchMe] == 0) {
	lastTouchRead [touchMe] = touch;
	lastTouchMin [touchMe] = touch;
  } else {
	long newTouch = lastTouchRead [touchMe] + (touch - lastTouchRead [touchMe]) / filterLength;
	if (abs (lastTouchRead[touchMe] - newTouch) > touchThreshold) {
	  if (lastTouchMin [touchMe] > touch) lastTouchMin [touchMe] = touch;

	  if (lastTouchRead[touchMe] > newTouch) {  // Descending, make it faster
		lastTouchRead[touchMe] = touch;
	  } else {
		lastTouchRead[touchMe] = newTouch;
	  }
	  return true;
	} else {
	  lastTouchRead[touchMe] = newTouch;
	  return false;
	}
  }
}

//! Switch us into testing mode, send periodic updates
void enableTestMode() {
  testing = true;
  Serial.println("TEST");
}

//! Switch us into production mode, stop simulated updates
void disableTestMode() {
  testing = false;
  Serial.println("PROD");
}

//! Process a command to change the sensor sensitivity
void processThresholdRequest() {
  int aNumber;  
  char *arg; 

  arg = SCmd.next(); 
  if (arg != NULL) {
    aNumber=atoi(arg);
    Serial.print("THRS:");
	Serial.println(aNumber); 
	touchThreshold = aNumber;
  } else {
    Serial.println("THRS:???");
  }
}

//! Process a command to change the filter length
void processFilterLengthRequest() {
  int aNumber;  
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) {
    aNumber=atoi(arg);
    Serial.print("FILT:");
	Serial.println(aNumber); 
	filterLength = aNumber;
  } else {
    Serial.println("FILT:???");
  }
}

//! Process a command to change the throttling value
void processThrottleRequest() {
  int aNumber;  
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) {
    aNumber=atoi(arg);
    Serial.print("THRT:");
	Serial.println(aNumber); 
	throttle = aNumber;
  } else {
    Serial.println("THRT:???");
  }
}

//! Process a command to get the settings values
void processSettingsRequest() {
  Serial.print("FILT:");
  Serial.println(filterLength); 
  Serial.print("THRS:");
  Serial.println(touchThreshold); 
  Serial.print("THRT:");
  Serial.println(throttle); 
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized() {
  Serial.println("UNKNOWN_COMMAND");
}

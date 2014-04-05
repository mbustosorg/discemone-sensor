#include "Arduino.h"
#include "SerialCommand.h"

// Available Touch Pins:
//   0, 1, 15, 16, 17, 18, 19, 22, 23

long lastTouchRead[22];
uint8_t touchPins[] = {0, 1, 15, 16, 17, 18, 19, 22, 23};
long TOUCH_THRESHOLD = 10;
bool testing = false;

SerialCommand SCmd;

// Prototypes
bool detectTouch (int touchMe);
void sendSensorData();
void enableTestMode();
void disableTestMode();
void processThresholdRequest();

//! Setup system
void setup()
{
  SCmd.addCommand("TESTMODE", enableTestMode);
  SCmd.addCommand("PRODMODE", disableTestMode);
  SCmd.addCommand("THRSHLD", processThresholdRequest);
  Serial.begin(57600);
}

//! Main loop
void loop()
{
  // Check to see if a command has been issued to us
  SCmd.readSerial();
  if (testing) {
	delay (1000);
	sendSensorData();
  } else {
	// Read sensors and update if appropriate
	bool sendUpdate = false;
	for (uint8_t i = 0; i < sizeof(touchPins); i++) {
	  sendUpdate = sendUpdate || detectTouch(i);
	}
	if (sendUpdate) {
	  sendSensorData();
	}
	delay(100);
  }
}

//! Send out our current sensor readings
void sendSensorData() {
  Serial.print ("SENSOR:");
  for (uint8_t i = 0; i < sizeof(touchPins); i++) {
	Serial.print(lastTouchRead[i]);
  }
  Serial.println("");
}

//! Have we detected a touch on pin `touchMe'?
bool detectTouch (int touchMe) {
  long touch = touchRead(touchMe);

  if (abs (lastTouchRead[touchMe] - touch) > TOUCH_THRESHOLD) {
	lastTouchRead[touchMe] = touch;
	return true;
  } else {
	return false;
  }
}

//! Switch us into testing mode, send periodic updates
void enableTestMode() {
  testing = true;
  Serial.println("TESTMODE");
}

//! Switch us into production mode, stop simulated updates
void disableTestMode() {
  testing = false;
  Serial.println("PRODMODE");
}

//! Process a command to change the sensor sensitivity
void processThresholdRequest() {
  int aNumber;  
  char *arg; 

  arg = SCmd.next(); 
  if (arg != NULL) {
    aNumber=atoi(arg);
    Serial.print("THRSHLD:");
	Serial.println(aNumber); 
  } else {
    Serial.println("THRSHLD:???");
  }
}

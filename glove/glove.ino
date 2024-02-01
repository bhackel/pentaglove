/*
  Arduino code for bluetooth glove project
*/

#include <ArduinoBLE.h>

#define POINTERF 3
#define MIDDLEF 4
#define RINGF 5
#define PINKYF 6

 // set the service and characteristic UUIDs
BLEService gloveService("180F");
BLEIntCharacteristic gloveStateChar("00002a19-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);

// set variables to track previous readings
int oldGloveState = 0;
long previousMillis = 0;

void setup() {
  // set pins to fingers
  pinMode(POINTERF, INPUT_PULLUP);
  pinMode(MIDDLEF, INPUT_PULLUP);
  pinMode(RINGF, INPUT_PULLUP);
  pinMode(PINKYF, INPUT_PULLUP);

  Serial.begin(9600);
  //while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // orange LED to indicate connection

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  /* Set the device name that will be advertised
     Set the Services, Characteristics, and starting value
  */
  BLE.setLocalName("gloveTransmitter");
  BLE.setAdvertisedService(gloveService);
  gloveService.addCharacteristic(gloveStateChar);
  BLE.addService(gloveService);
  gloveStateChar.writeValue(oldGloveState);

  // start advertising
  BLE.advertise();
  Serial.println("Glove active, waiting for connections...");
}

void loop() {
  // wait for a BLE central device to connect
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    // check the glove state every 200ms
    while (central.connected()) {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateGloveState();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateGloveState() {
  /* Set the state characteristic according to the current glove state
  */
  int gloveState = 0;
  // get current glove state
  if (digitalRead(POINTERF) == 0) {
    if (digitalRead(MIDDLEF) == 0) {
      gloveState = 5;
      delay(10);
    } else {
      gloveState = 1;
      delay(10);
    }
  }
  else if (digitalRead(MIDDLEF) == 0) {
    if (digitalRead(POINTERF) == 0) {
      gloveState = 5;
      delay(10);
    } else {
      gloveState = 2;
      delay(10);
    }
  }
  else if (digitalRead(RINGF) == 0) {
    if (digitalRead(PINKYF) == 0) {
      gloveState = 6;
      delay(10);
    } else {
      gloveState = 3;
      delay(10);
    }
  }
  else if (digitalRead(PINKYF) == 0) {
    if (digitalRead(RINGF) == 0) {
      gloveState = 6;
      delay(10);
    } else {
      gloveState = 4;
      delay(10);
    }
  }


  // update characteristic if data has changed
  if (gloveState != oldGloveState) {
    Serial.print("Glove state is now: ");
    Serial.println(gloveState);
    gloveStateChar.writeValue(gloveState);
    oldGloveState = gloveState;
  }
}

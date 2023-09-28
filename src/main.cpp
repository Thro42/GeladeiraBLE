#include <Arduino.h>

#include "config.h"
#include "BleSensorMain.h"

String ServiceUUID = "fff0";
String CharUUID = "fff2";
String deiviceName = "sps";
uint8_t deviceAddress[6] = {0x49,0x22,0x07,0x07,0x0b,0x02};
IBS_TYPE thSensor(deviceAddress);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("starting Bluetooth® Low Energy");
  thSensor.setupScan(ServiceUUID, CharUUID);
}

void loop() {
  thSensor.loop();
  if (thSensor.hasValues() )
  {
    float newTemp = thSensor.Temperature();
    float newHum = thSensor.Humidity();
    Serial.printf("Temperature %.2f | ",newTemp);
    Serial.printf("Humidity %.2f",newHum);
    Serial.println();
    delay(1000); // Delay a second between loops.
  } else {
    delay(500); // Delay a have second between loops.
  }
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    if (b < 16) {
      Serial.print("0");
    }

    Serial.print(b, HEX);
  }
}
//REMEMBER TO POWER CYCLE THE MODULE AFTER UPLOADING

#include <SoftwareSerial.h>

SoftwareSerial ezoSerial(2, 3);  // RX = pin 2, TX = pin 3 (adjust if needed)

void setup() {
  Serial.begin(9600);  // For Serial Monitor
  ezoSerial.begin(9600);  // Start communication with EZO sensor

  delay(1000);  // Allow time for sensor to initialize

  Serial.println("Sending I2C mode switch command...");
  
  // Send "i2c,1" command to switch the sensor to I2C mode
  ezoSerial.print("i2c,1\r");  // \r is required as a carriage return

  // Allow some time for the command to process
  delay(300);  

  // Read any response from the sensor
  Serial.println("Response from sensor:");
  while (ezoSerial.available()) {
    char c = ezoSerial.read();
    Serial.print(c);
  }
  Serial.println("\nMode switch complete. Power cycle the sensor.");
}

void loop() {
  // Nothing to do in the loop for this setup
}

/*
    FROM https://www.instructables.com/AUTOMATIC-TEMPERATURE-COMPENSATION-OF-THE-EZO-EC-S/

*/

#include <Ezo_i2c.h>                    //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>

#define RDT_ADDRESS 102
#define PH_ADDRESS 0x1

Ezo_board ph = Ezo_board(PH_ADDRESS, "PH");     //create a PH circuit object, who's address is 99 and name is "PH"
Ezo_board temperature = Ezo_board(RDT_ADDRESS, "TEMP"); //create an RTD Temperature circuit object who's address is 102 and name is "TEMP"
float temp;

void setup() {
  Wire.begin();                         //start the I2C
  Serial.begin(9600);                   //start the serial communication to the computer
}

void loop() {

  // check if there is a valid temperature reading
  if ((temperature.get_error() == Ezo_board::SUCCESS) && (temperature.get_last_received_reading() > -1000.0)) {

    //send readings from temp sensor to pH sensor
    ph.send_read_with_temp_comp( temperature.get_last_received_reading() );
  } else {
    ph.send_read_cmd();
  }
  temperature.send_read_cmd();

  delay(1000);                                          // read command needs a second to process

  receive_reading(ph);                                  //get the reading from the PH circuit
  Serial.print("  ");
  receive_reading(temperature);                         //get the reading from the RTD circuit
  Serial.println();
}

// function to decode the reading after the read command was issued
void receive_reading(Ezo_board &Sensor) {

  Serial.print(Sensor.get_name()); Serial.print(": ");  // print the name of the circuit getting the reading

  Sensor.receive_read_cmd();                            // get the response data

  switch (Sensor.get_error()) {                         // switch case based on what the response code is.
    case Ezo_board::SUCCESS:
      Serial.print(Sensor.get_last_received_reading()); //the command was successful, print the reading
      break;

    case Ezo_board::FAIL:
      Serial.print("Failed ");                          //means the command has failed.
      break;

    case Ezo_board::NOT_READY:
      Serial.print("Pending ");                         //the command has not yet been finished calculating.
      break;

    case Ezo_board::NO_DATA:
      Serial.print("No Data ");                         //the sensor has no data to send.
      break;
  }
}

// based on an example by Atlas Scientific
// https://github.com/Atlas-Scientific/Ezo_I2c_lib/blob/master/Examples/I2c_read_mulitple_circuits/I2c_read_mulitple_circuits.ino
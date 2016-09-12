// Copyright (c) 2016 Intel Corporation.  All rights reserved.
// See the bottom of this file for the license terms.

/*
 *  This sketch is to demonstrate I2C writing to a device, as a bus master.  It first makes a request to
 *  write to the device.  Then proceed with write out one byte annd completed the transmission.  The sketch
 *  continues with these steps and increament the byte value each lap.  If the write operation fails, please check
 *  the connection to the external device:  verify the signals, make sure the board and the device share a
 *  common ground connection.
 *
 *  Please note:
 *    1. This sketch makes use of ARC_I2C0.  One of the two I2C connections from the ARC core.  For the QUARK core I2C
 *       connection, please refer to the quarkI2C library.
 *    2. This sketch can easily be changed to make use of ARC_I2C1 - just change to the ARC_I2C1 object instead of ARC_I2C0.
 *    3. The ARC_I2C1 connections are not available in the Arduino 101 hardware platform.
 *    4. The sketch is accessing an external I2C device with an device number (address) of 8.  This address can be
 *       changed to fit your setup.
 */


#include <ARC_I2C.h>

void setup()
{
  Serial.begin(9600);   // start serial for output
  while(!Serial);       // Wait here till serial terminal is ready/opened

  ARC_I2C0.begin(); // join i2c bus
}

byte x = 1;
byte rdata;
void loop()
{
  ARC_I2C0.beginTransmission(8);  // transmit to device #8
  ARC_I2C0.write(x);              // sends one byte
  int result = ARC_I2C0.endTransmission();    // stop transmitting
  if (result == 0)
  {
    Serial.print("x =  ");
    Serial.println(x);
  }
  else
  {
    Serial.print("transmit failed with error code ");
    Serial.println(result);
  }
  x++;
  delay(500);
}


/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/


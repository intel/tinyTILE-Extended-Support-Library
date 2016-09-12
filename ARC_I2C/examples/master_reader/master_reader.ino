// Copyright (c) 2016 Intel Corporation.  All rights reserved.
// See the bottom of this file for the license terms.

/*
 *  This sketch is to demonstrate I2C reading from a device, as a bus master.  It first makes a request to
 *  read from the device.  Then proceed with reading out one byte at a time and dump in onto the serial term
 *  for display, in hex.  The sketch continue doing the above steps.  If the read operation fails, please check
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
  Serial.begin(9600);  // start serial for output
  while(!Serial);      // Wait here till serial terminal is ready/opened

  ARC_I2C0.begin();        // join i2c bus
}

void loop()
{

  int ret = ARC_I2C0.requestFrom(8, 20);    // request 20 bytes from slave device #8
  if (ret == 0)
  {
    Serial.println("read from slave device failed");
  }

  while (ARC_I2C0.available())   // slave may send less than requested
  {
    char c = ARC_I2C0.read(); // receive a byte as character
    Serial.println(c, HEX);
  }
  delay(1000);
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



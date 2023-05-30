// Copyright (c) 2016 Intel Corporation.  All rights reserved.
// See the bottom of this file for the license terms.

/*
 *  This sketch is to demonstrate I2C reading and writing as a I2C device, slave mode.  The sketch simply saves
 *  the data writen to it by a Master and provide it back when the Master reads it.  Notice that all the
 *  processing are done in event handlers - routines that are called, by the I2C library, upon the detection
 *  of an event (eg. bus master wants to write).
 *
 *  Consider an exciting setup as follow,
 *    1.  Have one board running this sketch as an I2C slave device.
 *    2.  Have another board running the sketch, master_check, as an I2C bus master.
 *    3.  The I2C signals of the two boards are connected together.  Please make sure they both share a common
 *        ground wire.
 *    4.  The serial term display of the two boards show the activities on the I2C bus.
 *
 *  Please note:
 *    1. This sketch makes use of I2C0.  One of the two I2C connections from the Quark core.  Only the Quark I2C
 *       interface has the slave mode capability.  For the ARC core I2C connection, please refer to the Wire library.
 *    2. This sketch can easily be changed to make use of I2C1 - just change to the I2C1 object instead of I2C0.
 *    3. The Quark I2C connections are not available in the Arduino 101 hardware platform.
 *    4. The sketch set the board to have a device number (address) of 8.  This address can be changed to fit your setup.
 */


#include <quarkI2C.h>

// MY_BUFFER_SIZE: the size of the buffer for storing data from Master.
// dataInBuffer: the amount of data received from master.
// myBuffer[MY_BUFFER_SIZE]: the data storage buffer.

#define MY_BUFFER_SIZE 32
static int dataInBuffer;
static uint8_t myBuffer[MY_BUFFER_SIZE];

void setup()
{
    Serial.begin(9600);  // start serial for output
    while (!Serial)      // Wait here till serial terminal is ready/opened
        ;
    I2C0.begin(0x180);  // join i2c bus with address #8
                        // slave device has to have an address
    // Call this routine when Master read from us.
    I2C0.onRequest(masterRequestMydataHandler);

    // Call this routine when Master write to us.
    I2C0.onReceive(masterSendMeDataHandler);
}

void loop()
{
}

// When I2C library detects a write request from the Master,
// this function is called to store up the data.
void masterSendMeDataHandler(int bytes)
{
    if (bytes > MY_BUFFER_SIZE)  // Can only take that much
      bytes = MY_BUFFER_SIZE;    // data from Master
    dataInBuffer = bytes;

    for (int i = 0; i < bytes; i++) {
        int x = I2C0.read();   // receive byte as an integer
        Serial.println(x, HEX); // print the integer
        myBuffer[i] = x;
    }
    Serial.print("slave receive ");
    Serial.print(bytes);
    Serial.println(" bytes");
}

// When I2C library detects a read request from the Master,
// this function is called to send data to Master which is
// a write operation in the point of view of a slave device.
void masterRequestMydataHandler()
{
    I2C0.write(myBuffer, dataInBuffer);
    Serial.print("slave transmit ");
    Serial.print(dataInBuffer);
    Serial.println(" bytes");
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





// Copyright (c) 2016 Intel Corporation.  All rights reserved.
// See the bottom of this file for the license terms.

/*
 *  This sketch is to demonstrate I2C reading and writing, to a device, as a bus master.  There 4 things it
 *  does:  Firstly, it generates a pattern. Secondly, it writes the pattern (data) to the device.  Thirdly, it reads
 *  the data back.  Finally, it compares between the read and write data to look for differences.  There should not
 *  be any differences.  Otherwise, it may point to a faulty I2C device or a bad connection between the board
 *  and the device (please check to see if the board and the device share a common ground).
 *
 *  Please note:
 *    1. This sketch makes use of I2C0.  One of the two I2C connections from the Quark core.  For the ARC core I2C
 *       connection, please refer to the Wire library.
 *    2. This sketch can easily be changed to make use of I2C1 - just change to the I2C1 object instead of I2C0.
 *    3. The Quark I2C connections are not available in the Arduino 101 hardware platform.
 *    4. The sketch is accessing an external I2C device with an device number (address) of 8.  This address can be
 *       changed to fit your setup.
 */

#include <quarkI2C.h>

const int MY_BUFFER_SIZE = 32;
const int I2C_DEVICE_ADDRESS = 8;    // Address of the I2C device the board is connected to

static int count = 0;
static uint8_t buffer_sender[MY_BUFFER_SIZE]; // data source for master writer
static uint8_t buffer_receiver[MY_BUFFER_SIZE]; // data distination for master reader

void setup() 
{
  I2C0.begin();         // join i2c bus 
  Serial.begin(9600);   // start serial for output
  while (!Serial)       // Wait here till serial terminal is ready/opened
    ;
}

void loop() 
{
  int ret = 0;
  int  bytesWritten = 0;

  count++;

  I2C0.beginTransmission(I2C_DEVICE_ADDRESS); // transmit to device #8

  // Step 1:  Generate the data pattern
  for (int i = 0; i < MY_BUFFER_SIZE; i++) 
  {
    buffer_sender[i] = i;
  }
  // Step 2:  Write the data pattern to the I2C device.
  bytesWritten = I2C0.write(buffer_sender, MY_BUFFER_SIZE); 

  ret = I2C0.endTransmission();    // stop transmitting
  if (ret != 0)
  {
      Serial.print("master write failed with error ");
      Serial.println(ret);
  }
  else
  {
      Serial.print("master sucessfull write ");
      Serial.print(bytesWritten);
      Serial.println(" bytes");
  }

  // Step 3: Read back the amount of written data from the device
  ret = I2C0.requestFrom(I2C_DEVICE_ADDRESS, bytesWritten, true);    // request MY_BUFFER_SIZE bytes from slave device #8    
  if (ret == 0)
  {
      Serial.println("master read failed");
  }
  else
  {
      Serial.print("master sucessfull read ");
      Serial.print(ret);
      Serial.println(" bytes");
  }

  int k = 0;
  while (I2C0.available())   // slave may send less than requested
  {
    buffer_receiver[k] = I2C0.read();
    k++;
  }

  // Step 4: check data - the received data should be equal to the sent data 
  for(int i = 0; i < bytesWritten; i++)
  {
    if(buffer_sender[i] == buffer_receiver[i])
    {
      /*Serial.println("OK");*/
    }
    else
    {
      Serial.print(buffer_sender[i],HEX);
      Serial.print("  !=  ");
      Serial.println(buffer_receiver[i],HEX);     
    }    
  }
  
  Serial.print("+++++");
  Serial.print(count);
  Serial.println("+++++");
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


// Copyright (c) 2016 Intel Corporation.  All rights reserved.
// See the bottom of this file for the license terms.

/*

    Please note:
      1. This sketch makes use of ARC_SPI0.  One of the two SPI connections from the ARC core.  For the QUARK core SPI
         connection, please refer to the SPI library.
      2. The ARC_SPI0 connections are not available in the Arduino 101 hardware platform.
      3. The expected Chip ID is 0x04
      4. 0xFF denotes connection errors
      5. If you need to read the dummy reg value you can do an immediate memory read
*/
// This example uses a FRAM SPI chip
// Dataset is here : https://cdn-shop.adafruit.com/datasheets/MB85RS64V-DS501-00015-4v0-E.pdf
// Op codes:
/* Name Description Op-code
   WREN Set Write Enable Latch 0000 0110B
   WRDI Reset Write Enable Latch 0000 0100B
   RDSR Read Status Register 0000 0101B
   WRSR Write Status Register 0000 0001B
   READ Read Memory Code 0000 0011B
   WRITE Write Memory Code 0000 0010B
   RDID Read Device ID 1001 1111B
*/
// The routing on the TT is as follows
/*  TPB29     SPI0_SS_MOSI
    TPB24     SPI0_SS_MISO
    TPB23     SPI0_SS_SCK
    TPB22     SPI0_SS_CS0
    TPB27     SPI0_SS_CS1
    TPB16     SPI0_SS_CS3
*/

// include the SPI library:
#include <ARC_SPI0.h>
const uint8_t dummy_reg = 0x7F;
void setup()
{


  Serial.begin(9600);  // start serial for output
  //while (!Serial) { };

  //Move pins to correct Test Point and call the correct macro
  //SPI_SE_1 = CS0 -> TBP22
  //SPI_SE_2 = CS1 -> TPB27
  //SPI_SE_4 = CS3 -> TBP16
  // default now to CS 0 uncomment and move pins as needed
  ARC_SPI0.begin(SPI_SE_1); // initialize SPI:
  //ARC_SPI0.begin(SPI_SE_2); // initialize SPI:
  //ARC_SPI0.begin(SPI_SE_4); // initialize SPI:

  ARC_SPI0.transfer(dummy_reg);
}

void loop()
{
  Serial.print("Transfer OpCode RDID. Return Value = 0x"); // print the character
  uint8_t chip_id = ARC_SPI0.transfer(0x9F);
  Serial.println(chip_id, HEX); // print the character
  delay(1000);

  //ARC_SPI0.transfer(dummy_reg);
  Serial.print("Transfer OpCode READ. Return Value = 0x"); // print the character
  uint8_t mem_Read = ARC_SPI0.transfer(0x3);
  Serial.println(mem_Read, HEX); // print the character
  delay(1000);

  Serial.print("Transfer OpCode RDSR. Return Value = 0x"); // print the character
  uint8_t status_Register = ARC_SPI0.transfer(0x5);
  Serial.println(status_Register, HEX); // print the character
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

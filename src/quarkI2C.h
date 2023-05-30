/*
 * quarkI2C.h is developed based upon the TwoWire.h module.
 *
 * TwoWire.h - TWI/I2C library for Linux Userspace
 * Copyright (c) 2013 Parav https://github.com/meanbot.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modifications made to support the two I2C connections of the Quark core
 * Copyright (C) 2016 Intel Corporation
 */

#ifndef quarkI2C_h
#define quarkI2C_h

#include "Stream.h"
#include "variant.h"
#include "intel_qrk_i2c.h"

const int QUARK_I2C_BUFFER_LENGTH = 32;

const int QUARKI2C_I2C_SPEED_SLOW = 1;
const int QUARKI2C_I2C_SPEED_FAST = 2;

const int I2C_SPEED_HS = 3;
const int I2C_ADDR_7Bit = 0;
const int I2C_ADDR_10Bit = 1;

class quarkI2C : public Stream
{
  public:
    quarkI2C(SOC_I2C_CONTROLLER controller_id);
    void begin();
    void begin(uint8_t, int i2c_speed = QUARKI2C_I2C_SPEED_FAST,
               int i2c_addr_mode = I2C_ADDR_7Bit);
    void begin(int, int i2c_speed = QUARKI2C_I2C_SPEED_FAST,
               int i2c_addr_mode = I2C_ADDR_7Bit);    
    void end();
    void setSpeed(uint32_t);
    void setAddressMode(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void (*)(int));
    void onRequest(void (*)(void));

    inline size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t)n);
    }
    using Print::write;

  private:
    uint8_t rxBuffer[QUARK_I2C_BUFFER_LENGTH];
    uint8_t rxBufferIndex;
    uint8_t rxBufferLength;

    uint8_t txAddress;
    uint8_t txBuffer[QUARK_I2C_BUFFER_LENGTH];
    uint8_t txBufferLength;

    void (*onRequestUserCallback)(void);
    void (*onReceiveUserCallback)(int);

    int init_status;
    SOC_I2C_CONTROLLER controllerId;

    static void onReceiveCallback(int, void *objectPtr);
    static void onRequestCallback(void *objectPtr);
};

extern quarkI2C I2C0;
extern quarkI2C I2C1;

#endif

/*
 * quarkI2C.cpp is developed based upon the TwoWire.cpp module.
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
 * Modifications to support Intel Arduino 101
 * Copyright (C) 2015 Intel Corporation
 *
 * Modifications made to support the two I2C connections of the Quark core
 * Copyright (C) 2016 Intel Corporation
 */

extern "C" {
#include <soc_i2c.h>
#include <string.h>
}

#include "quarkI2C.h"


quarkI2C::quarkI2C(SOC_I2C_CONTROLLER controller_id) : controllerId(controller_id)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txAddress = 0;
  txBufferLength = 0;

  onRequestUserCallback = NULL;
  onReceiveUserCallback = NULL;

  init_status = -1;
}

void quarkI2C::begin(void)
{
    int i2c_speed = QUARKI2C_I2C_SPEED_FAST;
    int i2c_addr_mode = I2C_ADDR_7Bit;
    init_status = soc_i2c_open_adapter(controllerId, 0, i2c_speed, i2c_addr_mode);
}

void quarkI2C::begin(uint8_t address, int i2c_speed, int i2c_addr_mode)
{
    if (address != 0) {
        init_status = soc_i2c_open_adapter(controllerId, address, i2c_speed, i2c_addr_mode);
        soc_i2c_slave_set_rx_user_buffer(controllerId, rxBuffer, (uint8_t)sizeof(rxBuffer));
    } else
        init_status = soc_i2c_open_adapter(controllerId, 0, i2c_speed, i2c_addr_mode);
}

void quarkI2C::begin(int address, int i2c_speed, int i2c_addr_mode)
{
    if (address != 0) {
        init_status = soc_i2c_open_adapter(controllerId, address, i2c_speed, i2c_addr_mode);
        soc_i2c_slave_set_rx_user_buffer(controllerId, rxBuffer, (uint8_t)sizeof(rxBuffer));
    } else
        init_status = soc_i2c_open_adapter(controllerId, 0, i2c_speed, i2c_addr_mode);
}

void quarkI2C::end()
{
    soc_i2c_close_adapter(controllerId);
}

void quarkI2C::setSpeed(uint32_t speed)
{
    soc_i2c_set_speed(controllerId, speed);
}

void quarkI2C::setAddressMode(uint32_t mode)
{
    soc_i2c_set_address_mode(controllerId, mode);
}

uint8_t quarkI2C::requestFrom(uint8_t address, uint8_t quantity,
                              uint8_t sendStop)
{
    int ret;
    if (quantity > QUARK_I2C_BUFFER_LENGTH)
        quantity = QUARK_I2C_BUFFER_LENGTH;

    /* Set slave address via ioctl  */
    soc_i2c_master_set_slave_address(controllerId, address);
    ret = soc_i2c_master_readbytes(controllerId, rxBuffer, quantity, !sendStop);
    if (ret < 0) {
        return 0;
    }
    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return quantity;
}

uint8_t quarkI2C::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t) true);
}

uint8_t quarkI2C::requestFrom(int address, int quantity, int sendStop)
{
    int ret;
    if (quantity > QUARK_I2C_BUFFER_LENGTH)
        quantity = QUARK_I2C_BUFFER_LENGTH;

    /* Set slave address via ioctl  */
    soc_i2c_master_set_slave_address(controllerId, address);
    ret = soc_i2c_master_readbytes(controllerId, rxBuffer, quantity, !sendStop);
    if (ret < 0) {
        return 0;
    }
    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return quantity;
}

uint8_t quarkI2C::requestFrom(int address, int quantity)
{
    return requestFrom((int)address, (int)quantity, (int)true);
}

void quarkI2C::beginTransmission(uint8_t address)
{
    if (init_status < 0)
        return;
    // set slave address
    soc_i2c_master_set_slave_address(controllerId, address);
    // reset transmit buffer
    txBufferLength = 0;
}

void quarkI2C::beginTransmission(int address)
{
    if (init_status < 0)
        return;
    // set slave address
    soc_i2c_master_set_slave_address(controllerId, address);
    // reset transmit buffer
    txBufferLength = 0;
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t quarkI2C::endTransmission(uint8_t sendStop)
{
    int err;
    // transmit buffer (blocking)
    if (txBufferLength >= 1) {
        err = soc_i2c_master_witebytes(controllerId, txBuffer, txBufferLength, !sendStop);
    } else {
        uint8_t temp = 0;
        // Workaround: I2C bus scan is currently implemented by reading,
        // so set the read length to 0 to inform the lower I2C driver that we
        // are doing bus scan
        err = soc_i2c_master_readbytes(controllerId, &temp, 0, 0);
    }
    // empty buffer
    txBufferLength = 0;
    if (err < 0) {
        return -err;
    }
    return 0; // success
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t quarkI2C::endTransmission(void)
{
    return endTransmission(true);
}

size_t quarkI2C::write(uint8_t data)
{
    if (txBufferLength >= QUARK_I2C_BUFFER_LENGTH)
        return 0;
    txBuffer[txBufferLength++] = data;
    return 1;
}

size_t quarkI2C::write(const uint8_t *data, size_t quantity)
{
    for (size_t i = 0; i < quantity; ++i) {
        if (txBufferLength >= QUARK_I2C_BUFFER_LENGTH)
            return i;
        txBuffer[txBufferLength++] = data[i];
    }
    return quantity;
}

int quarkI2C::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

int quarkI2C::read(void)
{
    if (rxBufferIndex < rxBufferLength)
        return rxBuffer[rxBufferIndex++];
    return -1;
}

int quarkI2C::peek(void)
{
    if (rxBufferIndex < rxBufferLength)
        return rxBuffer[rxBufferIndex];
    return -1;
}

void quarkI2C::flush(void)
{
    // Do nothing, use endTransmission(..) to force
    // data transfer.
}

void quarkI2C::onReceiveCallback(int bytes, void *objectPtr)
{
    quarkI2C *oPtr = (quarkI2C *)objectPtr;

    if (oPtr->onReceiveUserCallback == NULL) {
        return;
    }

    if (oPtr->rxBufferIndex < oPtr->rxBufferLength) {
        return;
    }

    oPtr->rxBufferIndex = 0;
    oPtr->rxBufferLength = bytes;

    oPtr->onReceiveUserCallback(bytes);
}

void quarkI2C::onRequestCallback(void *objectPtr)
{
    quarkI2C *oPtr = (quarkI2C *)objectPtr;

    if (oPtr->onRequestUserCallback == NULL) {
        return;
    }

    oPtr->txBufferLength = 0;

    oPtr->onRequestUserCallback();

    if (oPtr->txBufferLength >= 1) {
        soc_i2c_slave_set_tx_user_buffer(oPtr->controllerId, oPtr->txBuffer, oPtr->txBufferLength);
    }
}

void quarkI2C::onReceive(void (*function)(int))
{
    onReceiveUserCallback = function;
    soc_i2c_slave_set_rx_user_callback(controllerId, onReceiveCallback, (void *)this);
}

void quarkI2C::onRequest(void (*function)(void))
{
    onRequestUserCallback = function;
    soc_i2c_slave_set_tx_user_callback(controllerId, onRequestCallback, (void *)this);
}

quarkI2C I2C0 = quarkI2C(SOC_I2C_0);
quarkI2C I2C1 = quarkI2C(SOC_I2C_1);


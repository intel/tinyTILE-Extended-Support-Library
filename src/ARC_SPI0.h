/*
 * ARC_SPI0.h is developed based upon the TwoWire.h module.
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
 * Modifications made to support the two SPI connections of the ARC core
 * Copyright (C) 2016 Intel Corporation
 */

#ifndef _ARC_SPI_H_
#define _ARC_SPI_H_

#include "ss_spi.h"

#define LSBFIRST 0
#define MSBFIRST 1
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

class SPISettings
{
  public:
    SPISettings(uint32_t _speedMaximum, uint8_t _dataOrder, uint8_t _dataMode, SPI_SLAVE_ENABLE _spi_cs=SPI_SE_1)
        : speedMaximum(_speedMaximum), dataOrder(_dataOrder),
          dataMode((SPI_BUS_MODE)_dataMode), spi_cs(_spi_cs)
    {
    }

    SPISettings()
    {
        SPISettings(2000000, MSBFIRST, SPI_MODE0);
    }

  private:
    uint32_t speedMaximum;
    uint8_t dataOrder;
    SPI_BUS_MODE dataMode;
    SPI_SLAVE_ENABLE spi_cs;
    friend class ARC_SPI;
};

class ARC_SPI
{
public:
    ARC_SPI(SPI_CONTROLLER _controller_id) : controller_id(_controller_id)
    {
    }

    void begin(SPI_SLAVE_ENABLE spi_cs);
    void end();

    void beginTransaction(SPISettings settings);

    void setDataMode(uint8_t dataMode);
    void setClockDivider(uint8_t clockDiv);

    uint8_t transfer(uint8_t data);

  private:
    SPI_CONTROLLER controller_id;
};
extern ARC_SPI ARC_SPI0;

#endif

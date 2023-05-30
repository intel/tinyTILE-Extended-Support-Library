/*
 * ARC_SPI0.cpp is developed based upon the Wire.cpp module.
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
 * Modifications made to support the two SPI connections of the ARC core
 * Copyright (C) 2016 Intel Corporation
 */

#include "ARC_SPI0.h"

void ARC_SPI::begin(SPI_SLAVE_ENABLE spi_cs)
{
    ss_spi_init(controller_id, 2000, SPI_BUSMODE_0, SPI_8_BIT, spi_cs);
}

void ARC_SPI::end()
{
    ss_spi_disable(controller_id);
}

void ARC_SPI::beginTransaction(SPISettings settings)
{
    ss_spi_init(controller_id, settings.speedMaximum / 1000, settings.dataMode, SPI_8_BIT, settings.spi_cs);
}

uint8_t ARC_SPI::transfer(uint8_t data)
{
    uint8_t buffer[1];
    buffer[0] = data;
    ss_spi_xfer(controller_id, buffer, 1, 1);
    return buffer[0];
}

void ARC_SPI::setClockDivider(uint8_t clockDiv)
{
    ss_spi_set_clock_divider(controller_id, clockDiv);
}

void ARC_SPI::setDataMode(uint8_t dataMode)
{
    ss_spi_set_data_mode(controller_id, dataMode);
}

ARC_SPI ARC_SPI0 = ARC_SPI(SPI_SENSING_0);

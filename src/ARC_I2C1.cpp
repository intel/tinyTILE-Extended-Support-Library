/*
 * ARC_I2C1.cpp is developed based upon the Wire.cpp module.
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
 * Modifications made to support the two I2C connections of the ARC core
 * Copyright (C) 2016 Intel Corporation
 */


#include "ARC_I2C1.h"

TwoWire ARC_I2C1 = TwoWire(I2C_SENSING_1);


/*
    HeartBeatLed.cpp - Manage a heart-beat LED for CARRT-Pico

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "HeartBeatLed.h"

#include "pico/stdlib.h"

#include "CarrtPicoDefines.h"


namespace HeartBeatLed
{
    uint8_t ledState = 0;
}


void HeartBeatLed::initialize() noexcept
{
    gpio_init( CARRTPICO_HEARTBEAT_LED );
    gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

    // turn it on
    toggle();
}


void HeartBeatLed::toggle() noexcept
{
    gpio_put( CARRTPICO_HEARTBEAT_LED, HeartBeatLed::ledState );
    HeartBeatLed::ledState = !HeartBeatLed::ledState;
}


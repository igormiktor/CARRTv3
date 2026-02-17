/*
    Batteries.cpp - Functions for accessing battery voltages.

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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

/*!
 * \file Batteries.h
 *
 * \brief Implementation of functions for accessing battery voltages.
 *
 *
 * \author Igor Mikolic-Torreira (igormt@alumni.caltech.edu)
 * \copyright Copyright (c) 2026
 */

#include "Batteries.h"

#include <hardware/adc.h>
#include <hardware/gpio.h>
#include <pico/stdlib.h>

#include <cstdint>

#include "CarrtPicoDefines.h"

namespace Batteries
{
    constexpr float adcConversionFactor{ 3.3f / ( 1 << 12 ) };

    constexpr float icVoltageDividerFactor{ ( 39.f + 68.f ) / 68.f };
    constexpr float icConversionFactor{ icVoltageDividerFactor * adcConversionFactor };

    constexpr float motorVoltageDividerFactor{ ( 180.f + 82.f ) / 82.f };
    constexpr float motorConversionFactor{ motorVoltageDividerFactor * adcConversionFactor };
}    // namespace Batteries

void initBatteries()
{
    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init( CARRTPICO_IC_BATTERY_GPIO );
    adc_gpio_init( CARRTPICO_MOTOR_BATTERY_GPIO );
}

float Batteries::getIcBatteryVoltage()
{
    adc_select_input( CARRTPICO_IC_BATTERY_ADC );
    std::uint16_t rawValue = adc_read();
    float batteryVolts = icConversionFactor * rawValue;
    return batteryVolts;
}

float Batteries::getMotorBatteryVoltage()
{
    adc_select_input( CARRTPICO_MOTOR_BATTERY_ADC );
    std::uint16_t rawValue = adc_read();
    float batteryVolts = motorConversionFactor * rawValue;
    return batteryVolts;
}

/*
    CarrtPinAssignments.h - Pin assignments for CARRT's RPi GPIOs

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

#ifndef CarrtPinAssignments_h
#define CarrtPinAssignments_h

// clang-format off
namespace GpioPins
{
    constexpr auto kI2cSdaPin           = 2;
    constexpr auto kI2cClkPin           = 3;
    constexpr auto kBuzzerPin           = 12;
    constexpr auto kUartTxPin           = 14;
    constexpr auto kUartRxPin           = 15;
    constexpr auto kRpi0ShutdownPin     = 16;
    constexpr auto kPicoOnOffPin        = 17;
    constexpr auto kLowLiPoBattPin      = 23;
}
//clang-format on

#endif
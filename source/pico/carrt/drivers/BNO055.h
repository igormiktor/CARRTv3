/*
    BNO055.h - BNO055 functions for CARRT Pico.

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef BNO055_h
#define BNO055_h



namespace BNO055
{

    void initI2C();
    
    signed char send( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len );

    signed char receive( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len );

};




#endif  // BNO055_h


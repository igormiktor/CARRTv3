/*
    CarrtError.h - Base Error class for CARRT3

    Copyright (c) 2019 Igor Mikolic-Torreira.  All right reserved.

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






#ifndef CarrtError_h
#define CarrtError_h


#include <stdexcept>


class CarrtError : public std::runtime_error
{
public:

    explicit CarrtError( int errCode, const std::string& what )
        : std::runtime_error( what ), mErrorCode( errCode ) {}

    explicit CarrtError( int errCode, const char* what )
        : std::runtime_error( what ), mErrorCode( errCode ) {}

    int errorCode() const
        { return mErrorCode; }

private:

    int mErrorCode;
};


#endif  // CarrtError_h

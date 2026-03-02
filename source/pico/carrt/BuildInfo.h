/*
    BuildInfo.h - Contains build information for CARRTv3

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

#ifndef BuildInfo_h
#define BuildInfo_h

#include <cstdint>


namespace CarrtPicoVersion
{
    std::uint8_t major() noexcept;
    std::uint8_t minor() noexcept;
    std::uint8_t revision() noexcept;
    const char* versionStr() noexcept;

    const char* buildDate() noexcept;
    const char* buildTime() noexcept;
    std::uint32_t buildDateVal() noexcept;

    const char* features() noexcept;

    const char* hashFull() noexcept;
    const char* hashShort() noexcept;
    std::uint32_t hashShortVal() noexcept;
}

#endif    // BuildInfo_h

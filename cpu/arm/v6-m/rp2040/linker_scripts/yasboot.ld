/*
 yasboot.ld

 Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>

 This program is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation, either version
 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be
 useful, but WITHOUT ANY WARRANTY; without even the implied
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General
 Public License along with this program. If not, see
 <https://www.gnu.org/licenses/>.
*/

PROVIDE(YASBOOT_FLASH_START_ADDRESS = 0x10000200);
PROVIDE(YASBOOT_FLASH_SIZE = 0x10000);

/* 4 KB allocation for executed binary LOT */ 
PROVIDE(YASBOOT_RAM_START_ADDRESS = 0x2003c000);
PROVIDE(YASBOOT_RAM_SIZE = 16 * 1024);

PROVIDE(YASBOOT_RAM_LOT = 0x20000000);
PROVIDE(YASBOOT_RAM_LOT_SIZE = 0x1000);

PROVIDE(YASBOOT_RAM_APP = 0x20001000);
PROVIDE(YASBOOT_RAM_APP_SIZE = 252 * 1024);

INCLUDE "v6m_linker_script.ld"


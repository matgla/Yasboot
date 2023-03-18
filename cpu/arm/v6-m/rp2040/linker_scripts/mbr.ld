/*
  mbr_second_part.ld

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

/* we have 190 bytes for bootstrap code, my goal is to keep real MBR layout */ 
/* and some SRAM memory, we don't need a lot of it */ 
MEMORY {
  FLASH(rx) : ORIGIN = 0x10000100, LENGTH = 190
  SRAM(rwx) : ORIGIN = 0x20041f00, LENGTH = 2K
}

SECTIONS {
  . = ORIGIN(FLASH);
  .text : {
    *(.text)
  } > FLASH 
}


// 
// test.s
// 
// Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
// 
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General
// Public License along with this program. If not, see
// <https://www.gnu.org/licenses/>.

#include "hardware/regs/m0plus.h"
#include "hardware/regs/addressmap.h"

.syntax unified 
.cpu cortex-m0plus
.thumb 

.section .text 
.global bootstrap
.type bootstrap,%function 
.thumb_func
bootstrap:
  bkpt #1
  /* load vector table */ 
  ldr r0, =(XIP_BASE + 0x200)
  ldr r1, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
  /* and execute */
  str r0, [r1]
  ldmia r0, {r0, r1}
  msr msp, r0 
  bx r1 

  /* if child code is wrong, let's stay here */
loop:
  wfi 
  b loop


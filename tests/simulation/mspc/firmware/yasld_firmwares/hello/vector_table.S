// 
// vector_table.S 
// 
// Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
// 
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version. // // This program is distributed in the hope that it will be // useful, but WITHOUT ANY WARRANTY; without even the implied // warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General
// Public License along with this program. If not, see
// <https://www.gnu.org/licenses/>.

.syntax unified 
.cpu cortex-m0plus
.thumb 

#define PPB_BASE 0xe0000000
#define M0PLUS_VTOR_OFFSET 0x0000ed08
#define VECTOR_TABLE_SIZE 48

.section .ram_vector_table, "ax" 
.align 2 
.global __vector_table
__vector_table:
  .word 0x00000000 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq  
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq  
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 
  .word __unhandled_user_irq 


.text

.weak __unhandled_user_irq 
.type __unhandled_user_irq,%function
.thumb_func 
__unhandled_user_irq:
  bkpt #0 

.global hardware_init_hook
.type hardware_init_hook,%function 
.thumb_func 
hardware_init_hook:
  mov pc, lr

.global __register_frame_info
.type __register_frame_info,%function
.thumb_func 
__register_frame_info:
  mov pc, lr

.global __libc_fini
.type __libc_fini,%function 
.thumb_func 
__libc_fini:
  mov pc, lr

.global initialize_on_entry 
.type initialize_on_entry,%function
.thumb_func 
initialize_on_entry:
  mov r2, lr 
  adr r0, app_stack_pointer_symbol
  ldr r0, [r0]
  mov r1, r9
  ldr r0, [r1, r0]
  ldr r0, [r0]   
  msr msp, r0
  adr r0, __vector_table_symbol
  ldr r0, [r0]
  ldr r0, [r1, r0]
  ldr r1, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
  str r0, [r1]
  mov pc, r2  

.align 4
app_stack_pointer_symbol:
  .word __app_stack_pointer(GOT)
__vector_table_symbol:
  .word __vector_table(GOT)
  



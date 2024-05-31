// 
// vector_table.S 
// 
// Copyright (C) 2024 Mateusz Stadnik <matgla@live.com>
// 
// This program is free software: you can redistribute it and/or // modify it under the terms of the GNU General Public License
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

.global _entry 
.type _entry,%function
.thumb_func 
_entry:
  adr r4, app_stack_pointer_symbol
  ldr r4, [r4]
  mov r5, r9
  ldr r4, [r5, r4]
  ldr r4, [r4]   
  msr msp, r4
  adr r4, __vector_table_symbol
  ldr r4, [r4]
  ldr r4, [r5, r4]
  ldr r5, =(PPB_BASE + M0PLUS_VTOR_OFFSET)
  str r4, [r5]
  mov r5, r9
  adr r4, runtime_init_symbol
  ldr r4, [r4]
  ldr r4, [r5, r4]
  blx r4
  adr r4, main_symbol 
  ldr r4, [r4]
  ldr r4, [r5, r4]
  blx r4
  
  // we can't safely return from here 
  mov r0, r9 
  adr r1, _exit_symbol 
  ldr r1, [r1]
  ldr r1, [r0, r1]
  blx r1
  

.align 4
app_stack_pointer_symbol:
  .word __app_stack_pointer(GOT)
__vector_table_symbol:
  .word __vector_table(GOT)
main_symbol: 
  .word main(GOT)
_exit_symbol:
  .word _exit(GOT)
runtime_init_symbol:
  .word runtime_init(GOT)

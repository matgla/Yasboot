/*
 v6m_linker_script.ld

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

/* Memory section must be provided by board and CPU linker script */ 

ENTRY(_entry_point)

MEMORY 
{
  FLASH(rx) : ORIGIN = YASBOOT_FLASH_START_ADDRESS, LENGTH = YASBOOT_FLASH_SIZE
  HEAP(rwx) : ORIGIN = YASBOOT_HEAP, LENGTH = YASBOOT_HEAP_SIZE
  SRAM(rwx)  : ORIGIN = YASBOOT_RAM_START_ADDRESS, LENGTH = YASBOOT_RAM_SIZE
}

__heap_size__ = YASBOOT_HEAP_SIZE;

SECTIONS
{
  .vector_table : 
  {
    . = ALIGN(4);
    KEEP(*(.vector_table))
    . = ALIGN(4);
  } > FLASH 

  .text : 
  {
    . = ALIGN(4); 
    /* some of text sections would be better in RAM */ 
    *(.text) 
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    *(.init)
    *(.fini)
    . = ALIGN(4);
  } > FLASH

  .rodata : 
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4); 
  } > FLASH 
 
  /* ARM exception handling */ 
  .ARM.extab : 
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*) 
  } > FLASH
  .ARM.exidx : 
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .; 
  } > FLASH 

  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > FLASH

  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } > FLASH

  .fini_array : 
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } > FLASH
 
  __data_start_flash__ = LOADADDR(.data);
  .data : 
  {
    . = ALIGN(4);
    __data_start__ = .;
    *(vtable)
    *(.ram_function*)
    *(.data) 
    *(.data*)
    __data_end__ = .;
    . = ALIGN(4);

  } > SRAM AT> FLASH 

  .bss : 
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    __bss_end__ = .;
    . = ALIGN(4);
  } > SRAM

  .heap : 
  {
    . = ALIGN(4);
    __heap_start__ = .;
    . = . + __heap_size__;
    __heap_end__ = .;
    . = ALIGN(4);
  } > HEAP 
 
  _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}


/**
 * executable.ld
 *
 * Copyright (C) 2023 Mateusz Stadnik <matgla@live.com>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General
 * Public License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

MEMORY
{
  image(rwx) : ORIGIN = 0x00000000, LENGTH = 0xffffffff
}

ENTRY(main)

SECTIONS
{
  .text :
  {
    . = ALIGN(16);
    KEEP(*(.entry))
    *(.reset) 
    *(.text)
    *(.text*)
    *(.rel.text)
    *(.rel.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    . = ALIGN(4); 
    PROVIDE_HIDDEN (__mutex_array_start = .);
    KEEP(*(SORT(.mutex_array.*)))
    KEEP(*(.mutex_array))
    PROVIDE_HIDDEN (__mutex_array_end = .);
    . = ALIGN(4);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(SORT(.preinit_array.*)))
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  
    KEEP(*(.init))
    KEEP(*(.fini))
    *(.rodata)
    *(.rodata*)
    *(.ARM.extab*)
    *(.ARM.exidx*)
    *(.binary_info.*)
    . = ALIGN(16);
  } > image

  .data :
  {
    . = ALIGN(16);
    KEEP(*(.ram_vector_table))
    . = ALIGN(4);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN(__init_array_end = .);
    . = ALIGN(4);
    PROVIDE_HIDDEN(__fini_array_start = .);
    *(SORT(.fini_array.*))
    *(.fini_array)
    PROVIDE_HIDDEN(__fini_array_end = .);
 
    __data_start__ = .;

    *(.data)
    *(.data*)
    *(.time_critical)
    *(.time_critical*)
    . = ALIGN(16);
    __data_end__ = .;
  } > image 

  .bss :
  {
    . = ALIGN(16);
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(16);
  } > image

  .got :
  {
    *(.got*)
  } > image

  .dynsym :
  {
    *(.dynsym*)
  } > image 

  .dynstr :
  {
    *(.dynstr*)
  } > image

  .dynamic : 
  {
    *(.dynamic*)
  } > image

}

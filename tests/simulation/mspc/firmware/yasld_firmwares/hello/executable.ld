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
    . = ALIGN(8);
    KEEP(*(.vectors))
    KEEP(*(.entry))
    KEEP(*(.reset)) 
    *(.text)
    *(.text*)
    *(.rel.text)
    *(.rel.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP(*(.init))
    KEEP(*(.fini))
    *(.rodata)
    *(.rodata*)
    *(.ARM.extab*)
    *(.ARM.exidx*)
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*SORT(.preinit_array.*))
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN(__preinit_array_end = .);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*SORT(.init_array.*))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN(__init_array_end = .);
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*SORT(.fini_array.*))
    KEEP(*SORT(.fini_array))
    PROVIDE_HIDDEN(__fini_array_end = .);
    *(.binary_info.*)
    . = ALIGN(8);
  } > image

  .data :
  {
    . = ALIGN(8);
    __data_start__ = .;
    *(.ram_vector_table)
    PROVIDE_HIDDEN (__mutex_array_start = .);
    KEEP(*(SORT(.mutex_array.*)))
    KEEP(*(.mutex_array))
    PROVIDE_HIDDEN (__mutex_array_end = .);

    *(.data)
    *(.data*)
    *(.time_critical)
    *(.time_critical*)
    . = ALIGN(8);
    __data_end__ = .;
  } > image 

  .bss :
  {
    . = ALIGN(8);
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
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

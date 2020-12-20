/*
*  Copyright (C) 2010 Texas Instruments Incorporated. - http://www.ti.com/
*  All rights reserved.
*/

/*
 * ld script for SitaraWare AM335x
 */

/*
 * The starting address of DDR2 SDRAM is 0x80000000.
 * Though the DDR size is >128 MB, we keep the top of the stack at
 * 128 MB. Assumed that the (application + stack depth) < 128 MB
 * The Decremeting Stack top address = 0x87FFFFF8.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{

        . = 0x402F0400;
        . = ALIGN(4);
        .startcode     :
        {
               *init.o      (.text)
        }

        . = ALIGN(4);
        .text      :
        {
                *(.text)
        }

        . = ALIGN(4);

        .data :
        {
                *(.data)
        }
 
        . = ALIGN(4);

        _bss_start = .;
        .bss :
        {
                *(.bss)
        }
        . = ALIGN(4);

        _bss_end = .;

        _stack = 0x4030CDF8;
}


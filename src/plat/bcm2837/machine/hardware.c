/*
 * Copyright 2016, CSIRO Data61
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(D61_GPL)
 */


#include <config.h>
#include <types.h>
#include <arch/machine.h>
#include <arch/machine/timer.h>
#include <arch/linker.h>

/**
   DONT_TRANSLATE
 */
void resetTimer(void)
{
    resetGenericTimer();
}

/**
   DONT_TRANSLATE
 */
BOOT_CODE void
initTimer(void)
{
    initGenericTimer();
}

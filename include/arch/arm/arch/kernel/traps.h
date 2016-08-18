/*
 * Copyright 2016, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __KERNEL_ARM_TRAPS_H
#define __KERNEL_ARM_TRAPS_H

#include <config.h>
#include <util.h>

void c_handle_syscall(word_t cptr, word_t msgInfo, syscall_t syscall)
VISIBLE SECTION(".vectors.text");

void slowpath_irq(irq_t irq)
VISIBLE NORETURN SECTION(".vectors.text");

#endif /* __KERNEL_ARM_TRAPS_H */

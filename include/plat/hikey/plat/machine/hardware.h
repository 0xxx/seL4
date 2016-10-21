/*
 * Copyright 2016, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __PLAT_MACHINE_HARDWARE_H
#define __PLAT_MACHINE_HARDWARE_H

#include <config.h>
#include <basic_types.h>
#include <arch/linker.h>
#include <plat/machine.h>
#include <plat/machine/devices.h>

#define TIMER_CLOCK_HZ		1200000llu

#define physBase          0x20000000
#define kernelBase        0xe0000000

static const kernel_frame_t BOOT_RODATA kernel_devices[] = {
    {
        /*  GIC */
        GIC_CONTROLLER0_PADDR,
        GIC_CONTROLLER_PPTR,
        true  /* armExecuteNever */
    },
    {
        GIC_DISTRIBUTOR_PADDR,
        GIC_DISTRIBUTOR_PPTR,
        true  /* armExecuteNever */
#ifdef CONFIG_PRINTING
    },
    {
        /*  UART */
        UART0_PADDR,
        UART0_PPTR,
        true  /* armExecuteNever */
#endif
    }
};

/* Available physical memory regions on platform (RAM minus kernel image). */
/* NOTE: Regions are not allowed to be adjacent! */

static const p_region_t BOOT_RODATA avail_p_regs[] = {
    { .start = 0x20000000, .end = 0x28000000 }
};

static const p_region_t BOOT_RODATA dev_p_regs[] = {
    { /* .start = */ UART0_PADDR,    /* .end = */ UART0_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART1_PADDR,    /* .end = */ UART1_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART2_PADDR,    /* .end = */ UART2_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART3_PADDR,    /* .end = */ UART3_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ UART4_PADDR,    /* .end = */ UART4_PADDR + (1 << PAGE_BITS) },
    { /* .start = */ GIC_PADDR,      /* .end = */ GIC_PADDR + ((1 << PAGE_BITS) * 8) },
    { /* .start = */ DMTIMER0_PADDR, /* .end = */ DMTIMER0_PADDR + (1 << PAGE_BITS) },
};

/* Handle a platform-reserved IRQ. */
static inline void
handleReservedIRQ(irq_t irq)
{
}

#endif

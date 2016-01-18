/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#ifndef __LIBSEL4_ARCH_CONSTANTS_H
#define __LIBSEL4_ARCH_CONSTANTS_H

#include <autoconf.h>

#include <sel4/sel4_arch/constants.h>

#ifndef __ASM__
#include <sel4/sel4_arch/objecttype.h>
#include <sel4/arch/objecttype.h>
#endif

/* MSI IRQs need to be offset by this value in order to come
 * in along the right vector. Get the seL4_IRQHandler for
 * the irq number you want, then add IRQ_OFFSET to it when
 * programming the device */
#define IRQ_OFFSET 0x20

/* Range for MSI irqs. Currently no proper way of getting at the corresponding
 * definition inside seL4, but the vector table is setup such that MSIs start
 * after all the IRQs for the external interrupt controller (PIC or IOAPIC).
 * Regardless of whether the IOAPIC is used or not the MSI
 * block is after what would be the IOAPIC vector block. There are
 * 24 lines per IOAPIC */
#define MSI_MIN (24 * CONFIG_MAX_NUM_IOAPIC)
#define MSI_MAX (MSI_MIN + 0xd)

#if CONFIG_MAX_NUM_TRACE_POINTS > 0
/* size of kernel log buffer in bytes */
#define seL4_LogBufferSize (LIBSEL4_BIT(seL4_LargePageBits))
#endif /* CONFIG_MAX_NUM_TRACE_POINTS > 0 */

#endif

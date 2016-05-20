/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#ifndef __LIBSEL4_SEL4_ARCH_CONSTANTS_H
#define __LIBSEL4_SEL4_ARCH_CONSTANTS_H

enum {
    seL4_GlobalsFrame = 0xffffc000,
};

/* object sizes - 2^n */
#define seL4_PageBits 12
#define seL4_SlotBits 4
#define seL4_TCBBits 9
#define seL4_EndpointBits 4
#define seL4_NotificationBits 4
#ifdef ARM_HYP
#define seL4_PageTableBits      12
#else
#define seL4_PageTableBits 10
#endif
#define seL4_PageDirBits 14
#define seL4_ASIDPoolBits 12
#define seL4_ARM_VCPUBits       12
#define seL4_IOPageTableBits    12
/* word size */
#define seL4_WordBits (sizeof(seL4_Word) * 8)

#endif

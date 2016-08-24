/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_MACHINE_HARDWARE_H
#define __ARCH_MACHINE_HARDWARE_H

#include <types.h>
#include <config.h>
#include <arch/types.h>
#include <arch/linker.h>
#include <mode/api/constants.h>

#define PAGE_BITS seL4_PageBits
#define LARGE_PAGE_BITS seL4_LargePageBits

typedef word_t vm_fault_type_t;

enum vm_page_size {
    X86_SmallPage,
    X86_LargePage
};
typedef word_t vm_page_size_t;

/* Any changes to this function need to be replicated in pageBitsForSize_phys.
 */
static inline word_t CONST
pageBitsForSize(vm_page_size_t pagesize)
{
    switch (pagesize) {
    case X86_SmallPage:
        return seL4_PageBits;

    case X86_LargePage:
        return seL4_LargePageBits;

    default:
        fail("Invalid page size");
    }
}

/* This function is a duplicate of pageBitsForSize, needed for calls that occur
 * before the MMU is turned on. Note that any changes to this function need to
 * be replicated in pageBitsForSize.
 */
PHYS_CODE
static inline word_t CONST
pageBitsForSize_phys(vm_page_size_t pagesize)
{
    switch (pagesize) {
    case X86_SmallPage:
        return seL4_PageBits;

    case X86_LargePage:
        return seL4_LargePageBits;

    default:
        fail("Invalid page size");
    }
}

/* Returns the size of CPU's cacheline */
uint32_t CONST getCacheLineSize(void);
uint32_t CONST getCacheLineSizeBits(void);

/* Flushes a specific memory range from the CPU cache */
static inline void flushCacheLine(volatile void* vaddr)
{
    asm volatile ("clflush %[vaddr]" : [vaddr] "+m"(*((volatile char *)vaddr)));
}

void flushCacheRange(void* vaddr, uint32_t size_bits);

/* Disables a variety of prefetchers */
bool_t disablePrefetchers(void);

#endif

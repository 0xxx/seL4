/*
 * Copyright 2016, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(D61_GPL)
 */

#pragma once

/* Lower memory address to copy APs boot code in real mode */
#define BOOT_NODE_PADDR 0x80000

#if CONFIG_MAX_NUM_NODES > 1
void boot_node(void);
BOOT_CODE void start_boot_aps(void);
BOOT_CODE bool_t copy_boot_code_aps(uint32_t mem_lower);
#endif

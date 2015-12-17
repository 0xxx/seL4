/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_MODEL_STATEDATA_H
#define __ARCH_MODEL_STATEDATA_H

#include <config.h>
#include <types.h>
#include <util.h>
#include <object/structures.h>
#include <arch/types.h>
#include <plat/machine/devices.h>

#include <arch/object/iospace.h>
#include <plat/machine/hardware.h>

extern interrupt_t ia32KScurInterrupt;
extern tss_t ia32KStss;
extern gdt_entry_t ia32KSgdt[];
extern asid_pool_t* ia32KSASIDTable[];
extern tcb_t *ia32KSfpuOwner;
extern uint32_t ia32KScacheLineSizeBits;
extern idt_entry_t ia32KSidt[];
extern user_fpu_state_t ia32KSnullFpuState ALIGN(MIN_FPU_ALIGNMENT);
extern paddr_t ia32KSCurrentPD;
extern pdpte_t ia32KSGlobalPDPT[BIT(PDPT_BITS)];
extern pde_t ia32KSGlobalPD[BIT(PD_BITS + PDPT_BITS)];
extern pte_t ia32KSGlobalPT[BIT(PT_BITS)];

extern uint32_t ia32KSnumDrhu;
extern vtd_rte_t* ia32KSvtdRootTable;
extern uint32_t ia32KSnumIOPTLevels;
extern uint32_t ia32KSnumIODomainIDBits;

#if defined DEBUG || defined RELEASE_PRINTF
extern uint16_t ia32KSconsolePort;
extern uint16_t ia32KSdebugPort;
#endif

#endif

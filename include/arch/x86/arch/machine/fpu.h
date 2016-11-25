/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_KERNEL_FPU_H
#define __ARCH_KERNEL_FPU_H

#include <config.h>
#include <types.h>
#include <arch/object/vcpu.h>
#include <arch/machine.h>
#include <api/failures.h>

/* Initialise the FPU. */
bool_t Arch_initFpu(void);

/* Initialise the FPU state of the given user context. */
void Arch_initFpuContext(user_context_t *context);

/* Perform any actions required for the deletion of the given thread. */
void Arch_fpuThreadDelete(tcb_t *thread);

/* Handle an FPU exception. */
exception_t handleUnimplementedDevice(void);

void switchLocalFpuOwner(user_fpu_state_t *new_owner);

/* Switch the current owner of the FPU state on the core specified by 'cpu'. */
void switchFpuOwner(user_fpu_state_t *new_owner, word_t cpu);

static inline uint32_t xsave_features_high(void)
{
    uint64_t features = config_ternary(CONFIG_XSAVE, CONFIG_XSAVE_FEATURE_SET, 1);
    return (uint32_t)(features >> 32);
}

static inline uint32_t xsave_features_low(void)
{
    uint64_t features = config_ternary(CONFIG_XSAVE, CONFIG_XSAVE_FEATURE_SET, 1);
    return (uint32_t)(features & 0xffffffff);
}

/* Store state in the FPU registers into memory. */
static inline void saveFpuState(user_fpu_state_t *dest)
{
    if (config_set(CONFIG_FXSAVE)) {
        asm volatile("fxsave %[dest]" : [dest] "=m"(*dest));
    } else if (config_set(CONFIG_XSAVE_XSAVEOPT)) {
        asm volatile("xsaveopt %[dest]" : [dest] "=m"(*dest) : "d"(xsave_features_high()), "a"(xsave_features_low()));
    } else if (config_set(CONFIG_XSAVE_XSAVE)) {
        asm volatile("xsave %[dest]" : [dest] "=m"(*dest) : "d"(xsave_features_high()), "a"(xsave_features_low()));
    } else if (config_set(CONFIG_XSAVE_XSAVEC)) {
        asm volatile("xsavec %[dest]" : [dest] "=m"(*dest) : "d"(xsave_features_high()), "a"(xsave_features_low()));
    } else if (config_set(CONFIG_XSAVE_XSAVES)) {
        asm volatile("xsaves %[dest]" : [dest] "=m"(*dest) : "d"(xsave_features_high()), "a"(xsave_features_low()));
    }
}

/* Load FPU state from memory into the FPU registers. */
static inline void loadFpuState(user_fpu_state_t *src)
{
    if (config_set(CONFIG_FXSAVE)) {
        asm volatile("fxrstor %[src]" :: [src] "m"(*src));
    } else if (config_set(CONFIG_XSAVE)) {
        if (config_set(CONFIG_XSAVE_XSAVES)) {
            asm volatile("xrstors %[src]" :: [src] "m"(*src), "d"(xsave_features_high()), "a"(xsave_features_low()));
        } else {
            asm volatile("xrstor %[src]" :: [src] "m"(*src), "d"(xsave_features_high()), "a"(xsave_features_low()));
        }
    }
}

/* Reset the FPU registers into their initial blank state. */
static inline void finit(void)
{
    asm volatile("finit" :: "m"(control_reg_order));
}

/*
 * Enable the FPU to be used without faulting.
 * Required even if the kernel attempts to use the FPU.
 */
static inline void enableFpu(void)
{
    asm volatile("clts" :: "m" (control_reg_order));
}

/*
 * Disable the FPU so that usage of it causes a fault
 */
static inline void disableFpu(void)
{
    write_cr0(read_cr0() | CR0_TASK_SWITCH);
}

/* Returns whether or not the passed thread is using the current
 * active fpu state */
static inline bool_t nativeThreadUsingFPU(tcb_t *thread)
{
    return &thread->tcbArch.tcbContext.fpuState == ARCH_NODE_STATE_ON_CORE(x86KSActiveFPUState, thread->tcbAffinity);
}

#ifdef CONFIG_VTX
static inline bool_t vcpuThreadUsingFPU(tcb_t *thread)
{
    return thread->tcbArch.vcpu && &thread->tcbArch.vcpu->fpuState == ARCH_NODE_STATE(x86KSActiveFPUState);
}
#endif

static inline void FORCE_INLINE lazyFPURestore(tcb_t *thread)
{
    if (unlikely(ARCH_NODE_STATE(x86KSActiveFPUState))) {
        /* If we have enabled/disabled the FPU too many times without
         * someone else trying to use it, we assume it is no longer
         * in use and switch out its state
         */
        if (unlikely(ARCH_NODE_STATE(x86KSFPURestoresSinceSwitch) > CONFIG_X86_FPU_MAX_RESTORES_SINCE_SWITCH)) {
            switchLocalFpuOwner(NULL);
            ARCH_NODE_STATE(x86KSFPURestoresSinceSwitch) = 0;
        } else {
            if (likely(nativeThreadUsingFPU(thread))) {
                /* We are using the FPU, make sure it is enabled */
                enableFpu();
            } else {
                /* Someone is using the FPU and it might be enabled */
                disableFpu();
            }
            ARCH_NODE_STATE(x86KSFPURestoresSinceSwitch)++;
        }
    } else {
        /* No-one (including us) is using the FPU, so we assume it
         * is currently disabled */
    }
}

#endif

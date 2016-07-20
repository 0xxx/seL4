/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#ifndef __LIBSEL4_FAULTS_H
#define __LIBSEL4_FAULTS_H

#include <sel4/types.h>
#include <sel4/sel4_arch/faults.h>

static inline seL4_Fault_t
seL4_getFault(seL4_MessageInfo_t tag)
{

    switch (seL4_MessageInfo_get_label(tag)) {
    case seL4_Fault_CapFault:
        return seL4_Fault_CapFault_new(seL4_GetMR(seL4_CapFault_IP),
                                       seL4_GetMR(seL4_CapFault_Addr),
                                       seL4_GetMR(seL4_CapFault_InRecvPhase),
                                       seL4_GetMR(seL4_CapFault_LookupFailureType),
                                       seL4_GetMR(seL4_CapFault_BitsLeft),
                                       seL4_GetMR(seL4_CapFault_GuardMismatch_GuardFound),
                                       seL4_GetMR(seL4_CapFault_GuardMismatch_BitsFound));

    default:
        return seL4_getArchFault(tag);
    }
}

static inline seL4_Bool
seL4_isVMFault_tag(seL4_MessageInfo_t tag)
{
    return seL4_MessageInfo_get_label(tag) == seL4_Fault_VMFault;
}

static inline seL4_Bool
seL4_isUnknownSyscall_tag(seL4_MessageInfo_t tag)
{
    return seL4_MessageInfo_get_label(tag) == seL4_Fault_UnknownSyscall;
}

static inline seL4_Bool
seL4_isUserException_tag(seL4_MessageInfo_t tag)
{
    return seL4_MessageInfo_get_label(tag) == seL4_Fault_UserException;
}

static inline seL4_Bool
seL4_isNullFault_tag(seL4_MessageInfo_t tag)
{
    return seL4_MessageInfo_get_label(tag) == seL4_Fault_NullFault;
}

static inline seL4_Bool
seL4_isCapFault_tag(seL4_MessageInfo_t tag)
{
    return seL4_MessageInfo_get_label(tag) == seL4_Fault_CapFault;
}

#endif

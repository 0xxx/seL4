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

#ifdef HAVE_AUTOCONF
#include <autoconf.h>
#endif

#ifndef __ASSEMBLER__
enum {
    seL4_GlobalsFrame = 0xffffc000,
};
#endif

#define seL4_DataFault 0
#define seL4_InstructionFault 1

/* format of an unknown syscall message */
enum {
    seL4_UnknownSyscall_R0,
    seL4_UnknownSyscall_R1,
    seL4_UnknownSyscall_R2,
    seL4_UnknownSyscall_R3,
    seL4_UnknownSyscall_R4,
    seL4_UnknownSyscall_R5,
    seL4_UnknownSyscall_R6,
    seL4_UnknownSyscall_R7,
    seL4_UnknownSyscall_FaultIP,
    seL4_UnknownSyscall_SP,
    seL4_UnknownSyscall_LR,
    seL4_UnknownSyscall_CPSR,
    seL4_UnknownSyscall_Syscall,
    /* length of an unknown syscall message */
    seL4_UnknownSyscall_Length,
    SEL4_FORCE_LONG_ENUM(seL4_UnknownSyscall_Msg),
} seL4_UnknownSycall_Msg;

/* format of a user exception message */
enum {
    seL4_UserException_FaultIP,
    seL4_UserException_SP,
    seL4_UserException_CPSR,
    seL4_UserException_Number,
    seL4_UserException_Code,
    /* length of a user exception */
    seL4_UserException_Length,
    SEL4_FORCE_LONG_ENUM(seL4_UserException_Msg),
} seL4_UserException_Msg;

/* format of a vm fault message */
enum {
    seL4_VMFault_IP,
    seL4_VMFault_Addr,
    seL4_VMFault_PrefetchFault,
    seL4_VMFault_FSR,
    seL4_VMFault_Length,
    SEL4_FORCE_LONG_ENUM(seL4_VMFault_Msg),
} seL4_VMFault_Msg;

enum {
    seL4_TimeoutFault_Data,
    seL4_TimeoutFault_Consumed,
    /* consumed is 64 bits */
    seL4_TimeoutFault_Consumed_LowBits,
    seL4_TimeoutFault_Length,
    SEL4_FORCE_LONG_ENUM(seL4_TimeoutFault_Msg),
} seL4_TimeoutFault_Msg;

enum {
    seL4_TimeoutFaultReply_FaultIP,
    seL4_TimeoutFaultReply_SP,
    seL4_TimeoutFaultReply_CPSR,
    seL4_TimeoutFaultReply_R0,
    seL4_TimeoutFaultReply_R1,
    seL4_TimeoutFaultReply_R8,
    seL4_TimeoutFaultReply_R9,
    seL4_TimeoutFaultReply_R10,
    seL4_TimeoutFaultReply_R11,
    seL4_TimeoutFaultReply_R12,
    seL4_TimeoutFaultReply_R2,
    seL4_TimeoutFaultReply_R3,
    seL4_TimeoutFaultReply_R4,
    seL4_TimeoutFaultReply_R5,
    seL4_TimeoutFaultReply_R6,
    seL4_TimeoutFaultReply_R7,
    seL4_TimeoutFaultReply_R14,
    seL4_TimeoutFaultReply_Length,
    SEL4_FORCE_LONG_ENUM(seL4_TimeoutFaultReply_Msg)
} seL4_TimeoutFaultReply_Msg;

#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT
enum {
    seL4_VGICMaintenance_IDX,
    seL4_VGICMaintenance_Length,
    SEL4_FORCE_LONG_ENUM(seL4_VGICMaintenance_Msg),
} seL4_VGICMaintenance_Msg;

enum {
    seL4_VCPUFault_HSR,
    seL4_VCPUFault_Length,
    SEL4_FORCE_LONG_ENUM(seL4_VCPUFault_Msg),
} seL4_VCPUFault_Msg;

#endif /* CONFIG_ARM_HYPERVISOR_SUPPORT */

/* object sizes - 2^n */
#define seL4_PageBits 12
#define seL4_LargePageBits 16
#define seL4_SlotBits 4
#define seL4_TCBBits 9
#define seL4_EndpointBits 4
#define seL4_NotificationBits 5

#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT
#define seL4_PageTableBits      12
#define seL4_SectionBits 21
#define seL4_SuperSectionBits 25
#else
#define seL4_PageTableBits 10
#define seL4_SectionBits 20
#define seL4_SuperSectionBits 24
#endif

#define seL4_PageDirBits 14
#define seL4_ASIDPoolBits 12
#define seL4_SchedContextBits 6

#define seL4_ARM_VCPUBits       12
#define seL4_IOPageTableBits    12
/* word size */
#define seL4_WordBits (sizeof(seL4_Word) * 8)

#ifdef CONFIG_ENABLE_BENCHMARKS
/* size of kernel log buffer in bytes */
#define seL4_LogBufferSize (LIBSEL4_BIT(20))
#endif /* CONFIG_ENABLE_BENCHMARKS */

/* number of message registers passed in cpu registers */
#define seL4_FastMessageRegisters 4

#endif

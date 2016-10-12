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

#include <arch/machine/registerset.h>
#include <arch/machine/fpu.h>
#include <arch/object/structures.h>

const register_t msgRegisters[] = {
    R10, R8
};

const register_t frameRegisters[] = {
    FaultIP, RSP, FLAGS, RAX, RBX, RCX, RDX, RSI, RDI, RBP,
    R8, R9, R10, R11, R12, R13, R14, R15
};

const register_t gpRegisters[] = {
    TLS_BASE
};

const register_t exceptionMessage[] = {
    FaultIP, RSP, FLAGS
};

const register_t syscallMessage[] = {
    RAX, RBX, RCX, RDX, RSI, RDI, RBP, R8, R9, R10, R11, R12,
    R13, R14, R15, NextIP, RSP, FLAGS
};

void Mode_initContext(user_context_t* context)
{
    context->registers[RAX] = 0;
    context->registers[RBX] = 0;
    context->registers[RCX] = 0;
    context->registers[RDX] = 0;
    context->registers[RSI] = 0;
    context->registers[RDI] = 0;
    context->registers[RBP] = 0;
    context->registers[R8]  = 0;
    context->registers[R9]  = 0;
    context->registers[R10] = 0;
    context->registers[R11] = 0;
    context->registers[R12] = 0;
    context->registers[R13] = 0;
    context->registers[R14] = 0;
    context->registers[R15] = 0;
    context->registers[RSP] = 0;
}

word_t sanitiseRegister(register_t reg, word_t v)
{
    if (reg == FaultIP || reg == NextIP) {
        /* ensure instruction address is canonical */
        if (reg > 0x00007fffffffffff && reg < 0xffff800000000000) {
            /* no way to guess what the user wanted so give them zero */
            reg = 0;
        }
    }
    if (reg == FLAGS) {
        /* Set architecturally defined high and low bits */
        v |=  FLAGS_HIGH;
        v &= ~FLAGS_LOW;
        /* require user to have interrupts and no traps */
        v |=  FLAGS_IF;
        v &= ~FLAGS_TF;
        /* remove any other bits that shouldn't be set */
        v &=  FLAGS_MASK;
    }
    return v;
}

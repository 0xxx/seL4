/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#ifndef __ARCH_MODE_MACHINE_H_
#define __ARCH_MODE_MACHINE_H_

#include <config.h>
#include <arch/model/statedata.h>
#include <arch/machine/cpu_registers.h>
#include <arch/model/smp.h>

static inline cr3_t makeCR3(paddr_t addr, word_t pcid)
{
    return cr3_new(addr, config_set(CONFIG_SUPPORT_PCID) ? pcid : 0);
}

/* Address space control */
static inline cr3_t getCurrentCR3(void)
{
    return MODE_NODE_STATE(x64KSCurrentCR3);
}

static inline cr3_t getCurrentUserCR3(void)
{
    return getCurrentCR3();
}

static inline paddr_t getCurrentUserVSpaceRoot(void)
{
    return cr3_get_pml4_base_address(getCurrentUserCR3());
}

static inline void setCurrentCR3(cr3_t cr3, word_t preserve_translation)
{
    MODE_NODE_STATE(x64KSCurrentCR3) = cr3;
    word_t cr3_word = cr3.words[0];
    if (config_set(CONFIG_SUPPORT_PCID)) {
        if (preserve_translation) {
            cr3_word |= BIT(63);
        }
    } else {
        assert(cr3_get_pcid(cr3) == 0);
    }
    write_cr3(cr3_word);
}

/* there is no option for preservation translation when setting the user cr3
   as it is assumed you want it preserved as you are doing a context switch.
   If translation needs to be flushed then setCurrentCR3 should be used instead */
static inline void setCurrentUserCR3(cr3_t cr3)
{
    setCurrentCR3(cr3, 1);
}

static inline void setCurrentVSpaceRoot(paddr_t addr, word_t pcid)
{
    setCurrentCR3(makeCR3(addr, pcid), 1);
}

static inline void setCurrentUserVSpaceRoot(paddr_t addr, word_t pcid)
{
    setCurrentVSpaceRoot(addr, pcid);
}

/* GDT installation */
void x64_install_gdt(gdt_idt_ptr_t* gdt_idt_ptr);

/* IDT installation */
void x64_install_idt(gdt_idt_ptr_t* gdt_idt_ptr);

/* LDT installation */
void x64_install_ldt(uint32_t ldt_sel);

/* TSS installation */
void x64_install_tss(uint32_t tss_sel);

void handle_fastsyscall(void);

void init_syscall_msrs(void);

/* Get current stack pointer */
static inline void* get_current_esp(void)
{
    word_t stack;
    void *result;
    asm volatile("movq %[stack_address], %[result]" : [result] "=r"(result) : [stack_address] "r"(&stack));
    return result;
}

typedef struct invpcid_desc {
    uint64_t    asid;
    uint64_t    addr;
} invpcid_desc_t;

#define INVPCID_TYPE_ADDR           0
#define INVPCID_TYPE_SINGLE         1
#define INVPCID_TYPE_ALL_GLOBAL     2   /* also invalidate global */
#define INVPCID_TYPE_ALL            3

static inline void invalidateLocalPCID(word_t type, void *vaddr, asid_t asid)
{
    if (config_set(CONFIG_SUPPORT_PCID)) {
        invpcid_desc_t desc;
        desc.asid = asid & 0xfff;
        desc.addr = (uint64_t)vaddr;
        asm volatile ("invpcid %1, %0" :: "r"(type), "m"(desc));
    } else {
        switch (type) {
        case INVPCID_TYPE_ADDR:
            asm volatile("invlpg (%[vptr])" :: [vptr] "r"(vaddr));
            break;
        case INVPCID_TYPE_SINGLE:
        case INVPCID_TYPE_ALL:
            /* reload CR3 to perform a full flush */
            setCurrentCR3(getCurrentCR3(), 0);
            break;
        case INVPCID_TYPE_ALL_GLOBAL: {
            /* clear and reset the global bit to flush global mappings */
            unsigned long cr4 = read_cr4();
            write_cr4(cr4 & ~BIT(7));
            write_cr4(cr4);
        }
        break;
        }
    }
}

static inline void invalidateLocalTranslationSingle(vptr_t vptr)
{
    /* As this may be used to invalidate global mappings by the kernel,
     * and as its only used in boot code, we can just invalidate
     * absolutely everything form the tlb */
    invalidateLocalPCID(INVPCID_TYPE_ALL_GLOBAL, (void*)0, 0);
}

static inline void invalidateLocalTranslationSingleASID(vptr_t vptr, asid_t asid)
{
    invalidateLocalPCID(INVPCID_TYPE_ADDR, (void*)vptr, asid);
}

static inline void invalidateLocalTranslationAll(void)
{
    invalidateLocalPCID(INVPCID_TYPE_ALL_GLOBAL, (void*)0, 0);
}

static inline void invalidateLocalPageStructureCacheASID(paddr_t root, asid_t asid)
{
    if (config_set(CONFIG_SUPPORT_PCID)) {
        /* store our previous cr3 */
        cr3_t cr3 = getCurrentCR3();
        /* we load the new vspace root, invalidating translation for it
         * and then switch back to the old CR3. We do this in a single
         * asm block to ensure we only rely on the code being mapped in
         * the temporary address space and not the stack. We preserve the
         * translation of the old cr3 */
        asm volatile(
            "mov %[new_cr3], %%cr3\n"
            "mov %[old_cr3], %%cr3\n"
            ::
                [new_cr3] "r" (makeCR3(root, asid).words[0]),
                [old_cr3] "r" (cr3.words[0] | BIT(63))
        );
    } else {
        /* just invalidate the page structure cache as per normal, by
         * doing a dummy invalidation of a tlb entry */
        asm volatile("invlpg (%[vptr])" :: [vptr] "r"(0));
    }
}

static inline void swapgs(void)
{
    asm volatile("swapgs");
}

static inline rdmsr_safe_result_t x86_rdmsr_safe(const uint32_t reg)
{
    uint32_t low;
    uint32_t high;
    word_t returnto;
    word_t temp;
    rdmsr_safe_result_t result;
    asm volatile(
        "movabs $1f, %[temp] \n"
        "movq %[temp], (%[returnto_addr]) \n\
         rdmsr \n\
         1: \n\
         movq (%[returnto_addr]), %[returnto] \n\
         movq $0, (%[returnto_addr])"
        : [returnto] "=&r" (returnto),
        [temp] "=&r" (temp),
        [high] "=&d" (high),
        [low] "=&a" (low)
        : [returnto_addr] "r" (&ARCH_NODE_STATE(x86KSGPExceptReturnTo)),
        [reg] "c" (reg)
        : "memory"
    );
    result.success = returnto != 0;
    result.value = ((uint64_t)high << 32) | (uint64_t)low;
    return result;
}

#ifdef CONFIG_FSGSBASE_INST

static inline void x86_write_fs_base_impl(word_t base)
{
    asm volatile ("wrfsbase %0"::"r"(base));
}

static inline void x86_write_gs_base_impl(word_t base)
{
    asm volatile ("wrgsbase %0"::"r"(base));
}

static inline word_t x86_read_fs_base_impl(void)
{
    word_t base = 0;
    asm volatile ("rdfsbase %0":"=r"(base));
    return base;
}

static inline word_t x86_read_gs_base_impl(void)
{
    word_t base = 0;
    asm volatile ("rdgsbase %0":"=r"(base));
    return base;
}

#endif

#endif /* __ARCH_MODE_MACHINE_H_ */

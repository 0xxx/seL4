/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __MODEL_STATEDATA_H
#define __MODEL_STATEDATA_H

#include <types.h>
#include <object/structures.h>
#include <object/tcb.h>
#include <arch/model/statedata.h>
#include <arch/machine.h>

extern tcb_queue_t ksReadyQueues[] VISIBLE;
extern word_t ksReadyQueuesL1Bitmap[CONFIG_NUM_DOMAINS] VISIBLE;
extern word_t ksReadyQueuesL2Bitmap[CONFIG_NUM_DOMAINS][(CONFIG_NUM_PRIORITIES / wordBits) + 1] VISIBLE;
extern tcb_t *ksCurThread VISIBLE;
extern tcb_t *ksIdleThread VISIBLE;
extern tcb_t *ksSchedulerAction VISIBLE;
extern word_t ksWorkUnitsCompleted;
extern irq_state_t intStateIRQTable[] VISIBLE;
extern cte_t *intStateIRQNode VISIBLE;
extern const dschedule_t ksDomSchedule[];
extern const word_t ksDomScheduleLength;
extern word_t ksDomScheduleIdx;
extern dom_t ksCurDomain;
extern word_t ksDomainTime;
extern word_t tlbLockCount VISIBLE;

#ifdef CONFIG_BENCHMARK_USE_KERNEL_LOG_BUFFER
extern paddr_t ksUserLogBuffer;
#endif /* CONFIG_BENCHMARK_USE_KERNEL_LOG_BUFFER */

#define SchedulerAction_ResumeCurrentThread ((tcb_t*)0)
#define SchedulerAction_ChooseNewThread ((tcb_t*)~0)

#define NUM_READY_QUEUES (CONFIG_NUM_DOMAINS * CONFIG_NUM_PRIORITIES)

#endif

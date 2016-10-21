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

#ifndef __ARCH_MODEL_SMP_H_
#define __ARCH_MODEL_SMP_H_

#define ARCH_NODE_STATE(_state)             _state
#define MODE_NODE_STATE(_state)             _state
#define NODE_STATE_ON_CORE(_state, _core)   _state

#define NODE_STATE(_state)                  _state

#endif /* __ARCH_MODEL_SMP_H_ */

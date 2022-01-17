// Copyright (c) 2021 The CapableVMs "CHERI Examples" Contributors.
// SPDX-License-Identifier: MIT OR Apache-2.0

#include "main.h"

// Compartment functions
.global comp_f_fn
.global comp_g_fn

// Labels for size computations
.global comp_f_fn_end
.global comp_g_fn_end
.global switch_compartment_end

.text
.balign 4

.global executive_switch
.type executive_switch, "function"
executive_switch:
    mov      c29, c0
    mov      x0, #0
    cvtp     clr, lr
    b        switch_compartment
    ret      clr

#include "switch_compartment.s"

#include "compartments-try_deref.s"

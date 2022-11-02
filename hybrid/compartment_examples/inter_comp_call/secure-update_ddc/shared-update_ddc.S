// Copyright (c) 2021 The CapableVMs "CHERI Examples" Contributors.
// SPDX-License-Identifier: MIT OR Apache-2.0

.text
.balign 4

.global executive_switch
.type executive_switch, "function"
executive_switch:
    mov      c20, c0
    mov      c29, c0
    mov      x0, #0
    cvtp     clr, lr
    b        switch_compartment
    ret      clr


// `switch_compartment.S` conditionally includes one of two `clean`
// implementations. Setting `INTER_COMP_LEAK_X20` tells `switch-compartment.S`
// select the "broken" version of `clean` (which simulates leaking a register).
// In this example, x20 contains the DDC of the compartment switcher.
#define INTER_COMP_LEAK_X20 1
#include "../switch_compartment.S"

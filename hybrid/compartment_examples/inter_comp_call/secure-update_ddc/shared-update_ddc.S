// Copyright (c) 2021 The CapableVMs "CHERI Examples" Contributors.
// SPDX-License-Identifier: MIT OR Apache-2.0

.text
.balign 4

.global executive_switch
.type executive_switch, "function"
executive_switch:
    // c20 appears to be unused?
    mov      c20, c0
    mov      c29, c0
    mov      x0, #0
    // Yes, do not clean up x20 after returning from compartment switch
    mov      x13, #1
    cvtp     clr, lr
    b        switch_compartment
    ret      clr

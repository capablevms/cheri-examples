// Copyright (c) 2024 The CapableVMs "CHERI Examples" Contributors.
// See COPYRIGHT in the project root for details.
// SPDX-License-Identifier: Apache-2.0 OR MIT

// This demo is interactive, and should be self-explanatory in use. The code in
// this file is mostly related to display and infrastructure (such as drawing
// the backtrace).

#include <assert.h>
#include <cheriintrin.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void A(uint32_t locals);
void B(uint32_t locals);
void C(uint32_t locals);
void Z(uint32_t locals);
typedef void (*AbiHelperFn)(uint32_t);

bool is_in_A(uintptr_t pcc);
bool is_in_B(uintptr_t pcc);
bool is_in_C(uintptr_t pcc);
bool is_in_Z(uintptr_t pcc);

enum ABI
{
    AbiA,
    AbiB,
    AbiC,
    AbiZ,
    AbiUnknown,
};

const char *abi_label(enum ABI abi)
{
    switch (abi)
    {
    case AbiA:
        return "A";
    case AbiB:
        return "B";
    case AbiC:
        return "C";
    case AbiZ:
        return "Z";
    case AbiUnknown:
        return NULL;
    }
}

enum ABI abi_from_label(const char *label)
{
    if ((strcmp(label, "A") == 0) || (strcmp(label, "a") == 0))
    {
        return AbiA;
    }
    if ((strcmp(label, "B") == 0) || (strcmp(label, "b") == 0))
    {
        return AbiB;
    }
    if ((strcmp(label, "C") == 0) || (strcmp(label, "c") == 0))
    {
        return AbiC;
    }
    if ((strcmp(label, "Z") == 0) || (strcmp(label, "z") == 0))
    {
        return AbiZ;
    }
    return AbiUnknown;
}

const char *abi_desc(enum ABI abi)
{
    switch (abi)
    {
    case AbiA:
        return "C64-Secure, with naive (insecure) csp bounds restriction.";
    case AbiB:
        return "C64-Secure, with conservative csp bounds restriction.";
    case AbiC:
        return "C64-Secure v2, with precisely-restricted csp (and full cfp).";
    case AbiZ:
        return "AAPCS64-cap, used by C code.";
    case AbiUnknown:
        return "Unknown";
    }
}

enum ABI abi_for(size_t pcc)
{
    if (is_in_A(pcc))
    {
        return AbiA;
    }
    else if (is_in_B(pcc))
    {
        return AbiB;
    }
    else if (is_in_C(pcc))
    {
        return AbiC;
    }
    else if (is_in_Z(pcc))
    {
        return AbiZ;
    }
    else
    {
        return AbiUnknown;
    }
}

AbiHelperFn fn_for(enum ABI abi)
{
    switch (abi)
    {
    case AbiA:
        return A;
    case AbiB:
        return B;
    case AbiC:
        return C;
    case AbiZ:
        return Z;
    case AbiUnknown:
        return NULL;
    }
}

bool can_call(enum ABI from, enum ABI to)
{
    // Note: All ABIs can call C code well enough for `what_next()`, because
    // it doesn't need much stack. However, deep calls are likely to fail
    // without a trampoline to properly convert ABIs, so we don't permit
    // calls into AAPCS64-cap for this demo.
    switch (from)
    {
    case AbiA:
    case AbiB:
        return (to == AbiA) || (to == AbiB);
    case AbiC:
        return (to == AbiC);
    case AbiZ:
        // We have to be able to call anything from 'Z'
        // (AAPCS64-cap) because it's used by C code.
        return (to != AbiUnknown);
    case AbiUnknown:
        return false;
    }
}

int main(int argc, char *argv[])
{
    printf("C64-Secure Demo\n");
    Z(0);
    return 0;
}

bool is_in_range(size_t address, size_t start, size_t end)
{
    return (address >= start) && (address < end);
}

void clear()
{
    printf("\033[2J\033[H");
}

struct Context
{
    uintptr_t cfp;
    uintptr_t csp;
    uintptr_t pcc;
};

typedef size_t Slot;

// The left brackets show the stack frames, and mark the division between the
// locals and the frame records.
const char *lbracket_for(Slot slot, Slot csp_slot, Slot cfp_slot, Slot cfp_max_slot)
{
    if (slot == cfp_max_slot)
    {
        return "╭";
    }
    else if (slot == csp_slot)
    {
        return "╰";
    }
    else if (slot == cfp_slot)
    {
        return "├";
    }
    else if ((slot > csp_slot) && (slot < cfp_max_slot))
    {
        return "│";
    }
    else
    {
        return " ";
    }
}

// The right brackets show current (most recent) capabilities and how they
// relate to the stack. For all current ABIs, we draw csp and cfp.
const char *rbracket_for(Slot slot, Slot start_slot, Slot value_slot, Slot max_slot)
{
    bool is_value = slot == value_slot;
    if (slot == start_slot)
    {
        return is_value ? "◀╯" : " ╯";
    }
    else if (slot == max_slot)
    {
        return is_value ? "◀╮" : " ╮";
    }
    else if ((slot > start_slot) && (slot < max_slot))
    {
        return is_value ? "◀┤" : " │";
    }
    else
    {
        return is_value ? "◀ " : "  ";
    }
}

// Draw a backtrace of all demo functions, stopping when the backtrace one of
// the functions that we know about. In practice, that's whatever C code first
// called `Z(0)` or similar.
int backtrace(struct Context frame, struct Context const *current)
{
    enum ABI abi = abi_for(frame.pcc);
    if (abi == AbiUnknown)
    {
        // Probably C code. Stop looking for more frames.
        printf("  0: ...\n");
        return 0;
    }

    // We're going to display one slot per row, where (for simplicity) one
    // slot is the size of a single capability.
    const size_t slot_size_log2 = 4;
    const size_t slot_size = 1 << slot_size_log2;
    assert(slot_size == sizeof(uintptr_t)); // We require purecap.

    // The frame record starts at cfp for all PCS variants.
    const uintptr_t *frame_record = (const uintptr_t *) (frame.cfp);
    const char *frame_slot_names[3] = {"UNKNOWN", "UNKNOWN", "UNKNOWN"};
    size_t frame_record_slots;
    struct Context caller;
    size_t cfp_len = cheri_length_get(frame.cfp);
    ptraddr_t csp_limit = cheri_base_get(frame.csp) + cheri_length_get(frame.csp);
    switch (abi)
    {
    case AbiUnknown:
        // Probably C's `main`. Stop looking for more frames.
        printf("  0: ...\n");
        return 0;
    case AbiA:
    case AbiB:
        // C64-Secure frame
        assert(cheri_offset_get(frame.cfp) == 0);
        frame_record_slots = 3;
        caller.cfp = frame_record[0];
        caller.csp = frame_record[1];
        caller.pcc = frame_record[2];
        frame_slot_names[0] = "cfp";
        frame_slot_names[1] = "csp";
        frame_slot_names[2] = "pcc";
        break;
    case AbiC:
        // C64-Secure v2, with precisely-restricted csp.
        frame_record_slots = 2;
        ptraddr_t caller_sp = cheri_address_get(frame_record[0] + frame_record_slots);
        caller.cfp = cheri_address_set(frame.cfp, frame_record[0]);
        caller.csp = cheri_address_set(frame_record[0], caller_sp);
        caller.pcc = frame_record[1];
        frame_slot_names[0] = "(cfp x csp)";
        frame_slot_names[1] = "pcc";
        break;
    case AbiZ:
        // AAPCS64-cap frame
        frame_record_slots = 2;
        caller.cfp = frame_record[0];
        caller.csp = frame_record[0] + frame_record_slots;
        caller.pcc = frame_record[1];
        frame_slot_names[0] = "cfp";
        frame_slot_names[1] = "pcc";
        break;
    }

    // Recurse before printing this frame, so we print oldest frames first.
    int frame_num = backtrace(caller, current) + 1;

    // ---- Print the frame record. ----
    //
    // Do all arithmetic (etc) on 16-byte slots, to keep things simple. The
    // stack has to be 16-byte aligned at function boundaries so there
    // should be no corner cases here (at least with this demo).
    //
    // This code is simplified by the assumption that all test functions
    // emit their frame record first, immediately below their caller's stack
    // area. This is a common arrangement, sufficient for demonstration, but
    // not strictly required.

    Slot csp_slot = (Slot) (frame.csp) / slot_size;
    Slot cfp_slot = (Slot) (frame.cfp) / slot_size;
    Slot cfp_max_slot = cfp_slot + frame_record_slots - 1;

    size_t cur_csp = (size_t) (current->csp);
    size_t cur_csp_base = (size_t) (cheri_base_get(current->csp));
    size_t cur_csp_len = (size_t) (cheri_length_get(current->csp));
    Slot cur_csp_min_slot = cur_csp_base / slot_size;
    Slot cur_csp_slot = (Slot) (current->csp) / slot_size;
    Slot cur_csp_max_slot = (Slot) (cur_csp_base + cur_csp_len - 1) / slot_size;

    size_t cur_cfp_base = (size_t) (cheri_base_get(current->cfp));
    size_t cur_cfp_len = (size_t) (cheri_length_get(current->cfp));
    Slot cur_cfp_min_slot = cur_cfp_base / slot_size;
    Slot cur_cfp_slot = (Slot) (current->cfp) / slot_size;
    Slot cur_cfp_max_slot = (Slot) (cur_cfp_base + cur_cfp_len - 1) / slot_size;

    // Skip over locals, except some at the start and end of the range.
    const size_t print_locals = 9;
    size_t num_locals = cfp_slot - csp_slot;
    size_t skipped_locals_base = 0;
    size_t skipped_locals_limit = 0;
    if (num_locals > print_locals)
    {
        // Skip an extra line to allow for "...".
        size_t skip = num_locals - print_locals + 1;
        skipped_locals_base = csp_slot + (print_locals / 2);
        skipped_locals_limit = skipped_locals_base + skip;
    }

    for (Slot slot = cfp_max_slot; slot >= csp_slot; slot--)
    {
        size_t addr = slot * slot_size;
        char buf[128]; // For two-step formatting.

        if ((slot >= skipped_locals_base) && (slot < skipped_locals_limit))
        {
            slot = skipped_locals_base;
        }

        // Column 1: Function names and ownership brackets.
        const char *lbracket = lbracket_for(slot, csp_slot, cfp_slot, cfp_max_slot);
        if (slot == (cfp_max_slot - (frame_record_slots / 2)))
        {
            printf("%3d: %5s %s", frame_num, abi_label(abi), lbracket);
        }
        else
        {
            printf("           %s", lbracket);
        }

        // Column 2: Stack slot contents.
        printf(" %#18zx:", addr);
        if (slot >= cfp_slot)
        {
            // An entry in the frame record.
            size_t index = slot - cfp_slot;
            assert(index < frame_record_slots);
            snprintf(buf, sizeof(buf), "%s'%d", frame_slot_names[index], frame_num - 1);
        }
        else if ((slot >= skipped_locals_base) && (slot < skipped_locals_limit))
        {
            // Skipped locals (below the frame record).
            snprintf(buf, sizeof(buf), "...");
        }
        else
        {
            // Padding and locals (below the frame record).
            snprintf(buf, sizeof(buf), "local[%zu]", slot - csp_slot);
        }
        printf(" %-14s", buf);

        // Column 3: Current csp/cfp ranges.
        printf(" %s %s", rbracket_for(slot, cur_csp_min_slot, cur_csp_slot, cur_csp_max_slot),
               (slot == cur_csp_slot) ? "csp" : "   ");
        printf(" %s %s", rbracket_for(slot, cur_cfp_min_slot, cur_cfp_slot, cur_cfp_max_slot),
               (slot == cur_cfp_slot) ? "cfp" : "   ");

        printf("\n");
    }

    return frame_num;
}

struct Next
{
    uint32_t locals;
    uintptr_t fn;
};

struct Next next(uintptr_t what, size_t locals)
{
    struct Next result;
    result.locals = locals;
    result.fn = what;
    return result;
}

// If we can call a demo function that uses the specified ABI, then print it as
// an option for the user (and return true).
bool maybe_print_abi_desc(enum ABI from, enum ABI to)
{
    if (can_call(from, to))
    {
        char const *label = abi_label(to);
        char const *desc = abi_desc(to);
        printf("  %s: %s\n", (label ? label : "?"), (desc ? desc : "?"));
        return true;
    }
    return false;
}

// From a given context, ask the user what to call next.
//
// This is the interactive logic of the example, responsible for presenting
// available options, asking the user what to do next, and then passing that
// information back to the demo itself (A, B, C, ...). The actual ABI logic is
// in the demo functions.
//
// `what_next` should be called by each demo function (A, B, C, ...) in a loop.
// Each time it is called, `what_next` will prompt the user, and return another
// demo function to call (with a given number of stacked locals), or NULL if the
// calling demo function should return.
//
// Note: Since this is implemented in C, it uses AAPCS64-cap, which we
// respresent as 'Z'. `can_call()` says that the example ABIs can't call 'Z',
// but in practice they'll all work well enough to call this helper.
//
// NB: This might fail if the user requests a very large number of locals.
struct Next what_next(uintptr_t cfp, uintptr_t csp, size_t pc)
{
    clear();
    printf("C64-Secure Demo\n");
    struct Context ctx = {cfp, csp, pc};
    backtrace(ctx, &ctx);

    enum ABI from = abi_for(pc);

    printf("\n");
    printf("Enter a decimal number to allocate at least that many locals\n");
    printf("in the next called function, then a single character to choose\n");
    printf("the function to call:\n");
    maybe_print_abi_desc(from, AbiA);
    maybe_print_abi_desc(from, AbiB);
    maybe_print_abi_desc(from, AbiC);
    maybe_print_abi_desc(from, AbiZ);
    printf("  .: None (return).\n");

    // NULL means "return".
    const struct Next ret = {0, (uintptr_t) 0};
    size_t locals = 0;
    while (1)
    {
        int c = getchar();

        if (c == EOF)
        {
            exit(0);
        }

        if (c == '.')
        {
            return ret;
        }

        if (isspace(c))
        {
            continue;
        }

        char str[] = {c, '\0'};
        enum ABI abi = abi_from_label(str);
        if (can_call(from, abi))
        {
            return next((uintptr_t) fn_for(abi), locals);
        }

        bool bad_c = true;

        // Look for a digit.
        const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 0; i < (sizeof(digits) / sizeof(digits[0])); i++)
        {
            if (digits[i] == c)
            {
                locals = locals * 10 + i;
                bad_c = false;
                break;
            }
        }

        if (bad_c)
        {
            printf("Calling '%c' from '%s' is unimplemented.\n", toupper(c), abi_label(from));
            continue;
        }
    }
}

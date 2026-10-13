// break.cpp - Emulation activity control
// ---------
// Ported from np2debug (Author: Nmlgc) to np21w
//
// Differences from the np2debug original:
//  - np2active_renewal() is not reimplemented here; np21w already provides it
//    in win9x/np2.cpp (it also takes care of the sound muting), so
//    np2active_set() just drives the global np2break flag and calls it.
//  - The debug utility notification hooks (np2break_ui_*) replace np2debug's
//    viewmenu_all_debug_toggle() / viewstat_all_breakpoint().
//  - np2break_is_next() uses CPU_IP instead of CPU_EIP for the CS:IP lookup so
//    that it also works in 16-bit protected mode.

#include "compiler.h"

#include <string.h>
#include <stdlib.h>

#include "np2.h"
#include "unasm.h"
#include "unasmdef.tbl"
#include "cpucore.h"
#include "cpumem.h"
#include "break.h"

#if defined(SUPPORT_NP2BREAK)

// Size of the emulated conventional + high memory area (see i386c/cpumem.h).
// With HAXM/WHPX, mem is a pointer, so sizeof(mem) cannot be used here.
#define	NP2BREAK_MEMSIZE		0x200000

/// Globals
/// =======
BOOL	np2singlestep = 0;
np2break_t	np2breakflags[NP2BREAK_MEMSIZE];
LISTARRAY	np2breakaddrs = NULL;
/// =======

/// Activity
/// --------
void np2active_set(int active)
{
	UINT8	bk;

	if (active) {
		np2break_ui_resume();
	}

	// np2active_renewal() derives np2stopemulate from the global np2break
	// flag. A single step / resume has to run even if the main window
	// currently is inactive (NP2BREAK_MAIN), so temporarily override it and
	// restore the real state afterwards - exactly like np2debug's
	// np2active_renewal(NP2BREAK_RESUME) did.
	bk = np2break;
	np2break = (active) ? NP2BREAK_RESUME : NP2BREAK_DEBUG;
	np2active_renewal();
	np2break = bk;
}

void np2active_step(void)
{
	if (!np2stopemulate) {
		return;
	}
	np2active_set(1);
	np2singlestep = 1;
}

void np2active_step_over(void)
{
	UINT	step;
	_UNASM	una;

	if (!np2stopemulate) {
		return;
	}
	step = unasm_next(&una);
	if (!strcmp(una.mnemonic, "call") || !strcmp(una.mnemonic, "int")) {
		np2break_toggle(CPU_CS, CPU_IP + step, NP2BP_EXECUTE | NP2BP_ONESHOT);
		np2active_set(1);
	}
	else {
		np2active_step();
	}
}
/// --------

/// Breakpoints
/// -----------
void np2break_create(void)
{
	if (!np2breakaddrs) {
		np2breakaddrs = listarray_new(sizeof(UINT32), 16);
	}
}

static BOOL np2breakaddr_lookup_callback(void *vpItem, void *vpArg)
{
	return *((UINT32*)vpArg) == *((UINT32*)vpItem);
}

static UINT32* np2breakaddr_lookup(UINT32 addr)
{
	return (UINT32*)listarray_enum(np2breakaddrs, np2breakaddr_lookup_callback, &addr);
}

np2break_t* np2break_lookup_real(UINT32 addr)
{
	if (addr >= NP2BREAK_MEMSIZE) {
		// Out of the emulated memory - never a breakpoint.
		static np2break_t null_bp = NP2BP_NONE;
		return &null_bp;
	}
	return &np2breakflags[addr];
}

np2break_t* np2break_lookup(UINT32 *addr_if_hit, UINT16 seg, UINT16 off)
{
	UINT32 addr = (seg << 4) + off;
	np2break_t *ret = np2break_lookup_real(addr);
	if (*ret != NP2BP_NONE && addr_if_hit) {
		*addr_if_hit = addr;
	}
	return ret;
}

BOOL np2break_toggle_real(UINT32 addr, np2break_t flag)
{
	// LISTARRAY doesn't support element deletion.
	// We work around that here by zeroing out the elements that should be deleted.
	// Once a new element should be appended, we check for these zeroed entries
	// in order to keep the list from becoming larger and larger over time.
	UINT32* addr_slot = np2breakaddr_lookup(addr);
	np2break_t *flag_slot = np2break_lookup_real(addr);
	if (addr_slot && *flag_slot != NP2BP_NONE) {
		// Disable
		memset(addr_slot, 0, sizeof(*addr_slot));
		*flag_slot = NP2BP_NONE;
		return(TRUE);
	}

	// Look for a zeroed element
	addr_slot = np2breakaddr_lookup(0);
	if (!addr_slot) {
		// Nothing found, append a new one
		addr_slot = (UINT32*)listarray_append(np2breakaddrs, NULL);
		if (!addr_slot) {
			return(FALSE);
		}
	}
	// Set this element
	*addr_slot = addr;
	*flag_slot |= flag;
	return(TRUE);
}

BOOL np2break_toggle(UINT16 seg, UINT16 off, np2break_t flag)
{
	return np2break_toggle_real((seg << 4) + off, flag);
}

BOOL np2break_isactive(void)
{
	return (np2breakaddrs != NULL) && (listarray_getitems(np2breakaddrs) > 0);
}

static BOOL is_mem_type(const UNASM_MEMINFO *mi)
{
	if (!mi->off) {
		return FALSE;
	}
	switch (mi->type) {
		case OP_MEM:
		case OP_EA:
		case OP_PEA:
		case OP1_STR:
			return TRUE;
	}
	return FALSE;
}

// This is kept in here to catch edge cases not yet covered by unasm
static UINT32 np2break_memory_write_naive(void)
{
	static UINT bp_num = 0;
	static UINT8* probe = NULL;
	UINT bp_num_new = listarray_getitems(np2breakaddrs);
	UINT i;
	if (bp_num_new > bp_num) {
		UINT8* p = (UINT8*)realloc(probe, bp_num_new);
		if (!p) {
			return 0;
		}
		memset(p + bp_num, 0, bp_num_new - bp_num);
		probe = p;
		bp_num = bp_num_new;
	}
	for (i = 0; i < bp_num; i++) {
		UINT32* bp = (UINT32*)listarray_getitem(np2breakaddrs, i);
		if (!bp) {
			bp_num = i;
			return 0;
		}
		if (*bp && (*np2break_lookup_real(*bp) & NP2BP_WRITE)) {
			UINT8 cur_val = mem[*bp];
			if (probe[i] != cur_val) {
				probe[i] = cur_val;
				return *bp;
			}
		}
	}
	return 0;
}

UINT32 np2break_is_next(void)
{
	_UNASM	una;
	UINT32	addr = 0;
	np2break_t type = NP2BP_NONE;

#ifdef DEBUG
	addr = np2break_memory_write_naive();
	if (addr) {
		np2break_ui_breakpoint(NP2BP_WRITE, addr);
		return addr;
	}
#endif
	if (!listarray_getitems(np2breakaddrs)) {
		return 0;
	}

	unasm_next(&una);
	if (strcmp(una.mnemonic, "lea") && strcmp(una.mnemonic, "les")) {
		if (is_mem_type(&una.meminf[MI_READ])) {
			type |= *np2break_lookup(&addr, una.meminf[MI_READ].seg, una.meminf[MI_READ].off);
#ifndef DEBUG
		}
		else if (is_mem_type(&una.meminf[MI_WRITE])) {
			type |= *np2break_lookup(&addr, una.meminf[MI_WRITE].seg, una.meminf[MI_WRITE].off);
#endif
		}
	}
	if (type == NP2BP_NONE) {
		type |= *np2break_lookup(&addr, CPU_CS, CPU_IP);
	}
	if (type != NP2BP_NONE) {
		if (type & NP2BP_ONESHOT) {
			np2break_toggle_real(addr, 0);
		}
		np2break_ui_breakpoint(type, addr);
	}
	return addr;
}

void np2break_reset(void)
{
	memset(np2breakflags, 0, sizeof(np2breakflags));
	listarray_clr(np2breakaddrs);
}
void np2break_destroy(void)
{
	listarray_destroy(np2breakaddrs);
	np2breakaddrs = NULL;
}
/// -----------

/// Helper
/// ------
UINT unasm_next(UNASM una)
{
	UINT32	addr;
	UINT8	ins[16];

	addr = (CPU_STAT_PM ? CS_BASE : (CPU_CS << 4)) + CPU_IP;
	memp_reads(addr, ins, 16);
	return unasm(una, ins, 16, FALSE, addr);
}
/// ------

#endif	/* SUPPORT_NP2BREAK */

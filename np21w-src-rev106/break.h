// break.h - Emulation activity control
// ------
// Ported from np2debug (Author: Nmlgc) to np21w
//
// Provides the debugging activity control used by the Win9x debug utility:
// single-step execution, code/memory breakpoints and step-over.
//
// Note: NP2BREAK_MAIN / NP2BREAK_DEBUG are already declared by win9x/np2.h,
// so they are only defined here if that header was not included beforehand.

#ifndef NP2_BREAK_H__
#define NP2_BREAK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"
#include "common/lstarray.h"
#include "generic/unasm.h"

#ifndef NP2BREAK_FLAGS_DEFINED
enum {
	NP2BREAK_RESUME		= 0x00,
	NP2BREAK_MAIN		= 0x01,
	NP2BREAK_DEBUG		= 0x02
};
#define NP2BREAK_FLAGS_DEFINED
#endif

typedef enum {
	NP2BP_NONE		= 0x00,
	NP2BP_READ		= 0x01,
	NP2BP_WRITE		= 0x02,
	NP2BP_EXECUTE		= 0x04,
	NP2BP_ONESHOT		= 0x08
} np2bp_t;

typedef UINT8 np2break_t;

/// Debug utility UI hooks
/// ----------------------
// Implemented in win9x/np2.cpp and win9x/debuguty/viewer.cpp. They are always
// declared so that the activity control can be used from any translation unit,
// and they are safe to call even when no debug utility window is open.
void np2break_ui_resume(void);				// release input before resuming
void np2break_ui_toggle(BOOL bRunning);			// emulation started/stopped
void np2break_ui_breakpoint(np2break_t type, UINT32 addr);	// breakpoint hit
void np2break_ui_poll(void);				// UI thread: apply deferred state
/// ----------------------

// Last breakpoint hit. Written from the emulation thread (through
// np2break_ui_breakpoint), polled by the debug utility status window from the
// UI thread - the emulation thread must never touch the UI directly.
extern	np2break_t	np2break_lasthit_type;
extern	UINT32	np2break_lasthit_addr;

#if defined(SUPPORT_NP2BREAK)

/// Globals
/// -------
extern	UINT8	np2stopemulate;
extern	BOOL	np2singlestep;

// Dense list of flags for every possible address, used for quickly halting
// execution if necessary without having to traverse a linked list.
extern	np2break_t	np2breakflags[];
// Sparse linked list of all addresses with active breakpoints, used in the UI
// or for more naive checks.
extern	LISTARRAY	np2breakaddrs;
/// -------

/// Activity
/// --------
// Pauses (active == 0) or resumes (active != 0) the emulation for debugging.
void np2active_set(int active);
// Sets the single-step flag and resumes emulation.
// Next CPU loop will perform one instruction only, then pause the emulation again
void np2active_step(void);
// Like np2active_step(), but steps over CALL/INT instructions.
void np2active_step_over(void);
/// --------

/// Breakpoints
/// -----------
void np2break_create(void);

// These are guaranteed to return a valid, non-NULL pointer.
np2break_t* np2break_lookup_real(UINT32 addr);
np2break_t* np2break_lookup(UINT32 *addr_if_hit, UINT16 seg, UINT16 off);

BOOL np2break_toggle_real(UINT32 addr, np2break_t flag);
BOOL np2break_toggle(UINT16 seg, UINT16 off, np2break_t flag);

// TRUE if any breakpoint is currently set (used to bypass the
// multi-instruction execution fast path in the CPU core)
BOOL np2break_isactive(void);

// Returns the address of the breakpoint hit at the next instruction, or 0 for none
UINT32 np2break_is_next(void);

void np2break_reset(void);
void np2break_destroy(void);
/// -----------

/// Helper
/// ------
// Disassembles the instruction at CS:IP
UINT unasm_next(UNASM una);
/// ------

#else	/* !SUPPORT_NP2BREAK */

#define	np2singlestep		(0)
#define	np2break_isactive()		(0)

#endif	/* SUPPORT_NP2BREAK */

#ifdef __cplusplus
}
#endif

#endif	/* NP2_BREAK_H__ */

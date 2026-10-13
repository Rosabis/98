/**
 * @file	viewcmn.cpp
 * @brief	DebugUty ビュー共通�?��?の実�? (np2debug より移�?)
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewcmn.h"
#include "break.h"
#include "cpucore.h"

/// 表示補助
/// --------
void viewcmn_print_addr(LPTSTR lpString, UINT32 seg, UINT32 addr, BOOL bSegmented)
{
	if (bSegmented)
	{
		addr -= seg << 4;
		wsprintf(lpString, _T("%04x:%04x"), seg, addr);
	}
	else
	{
		wsprintf(lpString, _T("%08x"), addr);
	}
}

BOOL viewcmn_parse_hex(LPCTSTR lpString, UINT32* lpValue)
{
	UINT32 nValue = 0;
	BOOL bDigit = FALSE;

	if (lpString == NULL)
	{
		return FALSE;
	}
	while ((*lpString == _T(' ')) || (*lpString == _T('\t')))
	{
		lpString++;
	}
	while (*lpString)
	{
		TCHAR c = *lpString;
		UINT32 nDigit;
		if ((c >= _T('0')) && (c <= _T('9')))
		{
			nDigit = c - _T('0');
		}
		else if ((c >= _T('a')) && (c <= _T('f')))
		{
			nDigit = (c - _T('a')) + 10;
		}
		else if ((c >= _T('A')) && (c <= _T('F')))
		{
			nDigit = (c - _T('A')) + 10;
		}
		else
		{
			break;
		}
		nValue = (nValue << 4) | nDigit;
		bDigit = TRUE;
		lpString++;
	}
	while ((*lpString == _T(' ')) || (*lpString == _T('\t')))
	{
		lpString++;
	}
	if (!bDigit || (*lpString != _T('\0')))
	{
		return FALSE;
	}
	if (lpValue)
	{
		*lpValue = nValue;
	}
	return TRUE;
}
/// --------

#if defined(SUPPORT_NP2BREAK)

/// ブレークポイント関連
/// --------------------
BOOL viewcmn_has_breakpoint(UINT32 addr)
{
	return (*np2break_lookup_real(addr) != NP2BP_NONE);
}

TCHAR viewcmn_breakpoint_char(UINT32 addr)
{
	switch (*np2break_lookup_real(addr) & (NP2BP_READ | NP2BP_WRITE | NP2BP_EXECUTE))
	{
		case NP2BP_READ:
			return _T('R');
		case NP2BP_WRITE:
			return _T('W');
		case NP2BP_EXECUTE:
			return _T('X');
	}
	return _T(' ');
}

void viewcmn_toggle_breakpoint(UINT32 addr)
{
	np2break_t flag = *np2break_lookup_real(addr);
	if (flag & NP2BP_EXECUTE)
	{
		// 実行ブレークポイント�?�読み書きも兼ね�?
		np2break_toggle_real(addr, NP2BP_READ | NP2BP_WRITE);
	}
	else
	{
		np2break_toggle_real(addr, NP2BP_EXECUTE | NP2BP_READ | NP2BP_WRITE);
	}
}
/// --------------------

#endif	/* SUPPORT_NP2BREAK */

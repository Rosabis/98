/**
 * @file	d_addr.cpp
 * @brief	アドレス入力ダイアログの実装 (np2debug より移植)
 */

#include "compiler.h"
#include "common.h"
#include "resource.h"
#include "np2.h"
#include "cpucore.h"
#include "milstr.h"
#include "..\misc\WndProc.h"
#include "d_addr.h"

/// 16/32 ビットレジスタ名
/// --------------------
static const LPCTSTR s_regNames[] =
{
	_T("ax"),  _T("bx"),  _T("cx"),  _T("dx"),
	_T("sp"),  _T("bp"),  _T("si"),  _T("di"),
	_T("eax"), _T("ebx"), _T("ecx"), _T("edx"),
	_T("esp"), _T("ebp"), _T("esi"), _T("edi"),
	_T("es"),  _T("cs"),  _T("ss"),  _T("ds"),
	_T("fs"),  _T("gs"),
	_T("ip"),  _T("eip")
};

/// レジスタ名から現在値を得る
/// -------------------------
static UINT32 daddr_regvalue(int nIndex)
{
	switch (nIndex)
	{
		case  0: return (UINT32)CPU_AX;
		case  1: return (UINT32)CPU_BX;
		case  2: return (UINT32)CPU_CX;
		case  3: return (UINT32)CPU_DX;
		case  4: return (UINT32)CPU_SP;
		case  5: return (UINT32)CPU_BP;
		case  6: return (UINT32)CPU_SI;
		case  7: return (UINT32)CPU_DI;
		case  8: return (UINT32)CPU_EAX;
		case  9: return (UINT32)CPU_EBX;
		case 10: return (UINT32)CPU_ECX;
		case 11: return (UINT32)CPU_EDX;
		case 12: return (UINT32)CPU_ESP;
		case 13: return (UINT32)CPU_EBP;
		case 14: return (UINT32)CPU_ESI;
		case 15: return (UINT32)CPU_EDI;
		case 16: return (UINT32)CPU_ES;
		case 17: return (UINT32)CPU_CS;
		case 18: return (UINT32)CPU_SS;
		case 19: return (UINT32)CPU_DS;
		case 20: return (UINT32)CPU_FS;
		case 21: return (UINT32)CPU_GS;
		case 22: return (UINT32)CPU_IP;
		case 23: return (UINT32)CPU_EIP;
	}
	return 0;
}

/// レジスタ名あるいは 16 進数を解決する
static UINT32 daddr_solve(LPCTSTR lpszString)
{
	for (int i = 0; i < NELEMENTS(s_regNames); i++)
	{
		if (!_tcsicmp(lpszString, s_regNames[i]))
		{
			return daddr_regvalue(i);
		}
	}
	return (UINT32)milstr_solveHEX(lpszString);
}

enum
{
	SOURCE_SEG_OFF,
	SOURCE_REAL
};

static bool daddr_blockchange = false;
static UINT32 daddr_seg, daddr_off;

/// seg:off → real アドレスを表示に反映する
static void daddr_calc(HWND hWnd)
{
	TCHAR work[32];
	UINT32 real;

	daddr_blockchange = true;
	GetDlgItemText(hWnd, IDC_ADDR_SEG, work, 5);
	daddr_seg = daddr_solve(work);
	GetDlgItemText(hWnd, IDC_ADDR_OFF, work, 5);
	daddr_off = daddr_solve(work);
	real = (daddr_seg << 4) + daddr_off;
	wsprintf(work, _T("%05x"), real);
	SetDlgItemText(hWnd, IDC_ADDR_REAL, work);
	daddr_blockchange = false;
}

static INT_PTR CALLBACK daddr_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR work[8];
	int len;

	if (daddr_blockchange)
	{
		return FALSE;
	}
	switch (msg)
	{
		case WM_INITDIALOG:
			wsprintf(work, _T("%04x"), (UINT16)(UINT)daddr_seg);
			SetDlgItemText(hWnd, IDC_ADDR_SEG, work);
			wsprintf(work, _T("%04x"), (UINT16)(UINT)daddr_off);
			SetDlgItemText(hWnd, IDC_ADDR_OFF, work);
			daddr_calc(hWnd);
			SendDlgItemMessage(hWnd, IDC_ADDR_SEG, EM_SETLIMITTEXT, 4, NULL);
			SendDlgItemMessage(hWnd, IDC_ADDR_OFF, EM_SETLIMITTEXT, 4, NULL);
			SendDlgItemMessage(hWnd, IDC_ADDR_REAL, EM_SETLIMITTEXT, 5, NULL);
			SetFocus(GetDlgItem(hWnd, IDC_ADDR_SEG));
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hWnd, (daddr_seg << 16) + daddr_off);
					return TRUE;

				case IDCANCEL:
					EndDialog(hWnd, (UINT32)-1);
					return TRUE;
			}

			switch (HIWORD(wParam))
			{
				case EN_UPDATE:
					daddr_blockchange = true;
					GetDlgItemText(hWnd, LOWORD(wParam), work, 6);
					len = lstrlen(work);
					// セグメントが埋まったらオフセットへ進む
					if ((len == 4) && (LOWORD(wParam) == IDC_ADDR_SEG))
					{
						SendMessage(hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWnd, IDC_ADDR_OFF), TRUE);
					}
					daddr_blockchange = false;
					break;

				case EN_CHANGE:
					if (!daddr_blockchange)
					{
						daddr_calc(hWnd);
						return TRUE;
					}
					return FALSE;
			}
			break;
	}
	return FALSE;
}

UINT32 daddr_input(HWND hParent, UINT16 nSeg, UINT16 nOff)
{
	daddr_seg = nSeg;
	daddr_off = nOff;
	daddr_blockchange = false;

	HINSTANCE hInstance = CWndProc::GetResourceHandle();
	return (UINT32)DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDRESS), hParent, daddr_proc);
}

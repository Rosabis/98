/**
 * @file	d_edit.cpp
 * @brief	メモリ編集 / 検索ダイアログの実装 (np2debug より移植)
 */

#include "compiler.h"
#include "common.h"
#include "resource.h"
#include "np2.h"
#include "milstr.h"
#include "..\misc\WndProc.h"
#include "d_edit.h"

static const UINT16 s_editTypes[] =
{
	IDC_EDIT_TYPE_HEX,
	IDC_EDIT_TYPE_SJIS, IDC_EDIT_TYPE_UTF8
};

static const DWORD s_editCodepages[] =
{
	0,
	932, CP_UTF8
};

static EDITDATA s_editData = { { 0 }, { 0 }, 0, IDC_EDIT_TYPE_HEX };
static EDITDATA s_findData = { { 0 }, { 0 }, 0, IDC_EDIT_TYPE_HEX };

/// 16 進文字列をバイト列に変換する
/// -----------------------------
//! 空白は読み飛ばし、16 進桁 2 つで 1 バイト。奇数桁目は無視される。
//! @return 変換されたバイト数
static int dedit_hexbuf(UINT8* lpBuffer, int nBufferSize, LPCTSTR lpszString)
{
	int nLen = 0;
	int nDigits = 0;
	UINT nValue = 0;

	while (*lpszString)
	{
		TCHAR c = *lpszString++;
		UINT nDigit;

		if ((c >= _T('0')) && (c <= _T('9')))
		{
			nDigit = (UINT)(c - _T('0'));
		}
		else if ((c >= _T('a')) && (c <= _T('f')))
		{
			nDigit = (UINT)(c - _T('a')) + 10;
		}
		else if ((c >= _T('A')) && (c <= _T('F')))
		{
			nDigit = (UINT)(c - _T('A')) + 10;
		}
		else
		{
			continue;
		}

		nValue = (nValue << 4) | nDigit;
		nDigits++;
		if ((nDigits & 1) == 0)
		{
			if (nLen >= nBufferSize)
			{
				break;
			}
			lpBuffer[nLen++] = (UINT8)(nValue & 0xff);
			nValue = 0;
		}
	}
	return nLen;
}

/// ダイアログの文字列をバイト列に変換する
static void dedit_str_to_bytes(EDITDATA* lpData, HWND hWnd, int nItem)
{
	GetDlgItemText(hWnd, nItem, lpData->str, MAX_EDIT_STR);
	if (lpData->codepage)
	{
		// 終端の null は除く
		lpData->bytes_len = WideCharToMultiByte(lpData->codepage, 0, lpData->str, -1,
			(LPSTR)lpData->bytes, MAX_EDIT_STR * 3, NULL, NULL) - 1;
	}
	else
	{
		lpData->bytes_len = dedit_hexbuf(lpData->bytes, MAX_EDIT_STR * 3, lpData->str);
	}
}

/// ラジオボタンの選択をデータ型に反映する
static void dedit_get_type(EDITDATA* lpData, HWND hWnd)
{
	for (int i = 0; i < NELEMENTS(s_editTypes); i++)
	{
		if (SendDlgItemMessage(hWnd, s_editTypes[i], BM_GETCHECK, 0, 0) != 0)
		{
			lpData->type = s_editTypes[i];
			lpData->codepage = s_editCodepages[i];
		}
	}
}

static INT_PTR CALLBACK dedit_base_proc(EDITDATA* lpData, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			SendDlgItemMessage(hWnd, IDC_EDIT_STR, EM_SETLIMITTEXT, MAX_EDIT_STR - 1, NULL);
			SendDlgItemMessage(hWnd, lpData->type, BM_SETCHECK, BST_CHECKED, 0);
			SetDlgItemText(hWnd, IDC_EDIT_STR, lpData->str);
			SendDlgItemMessage(hWnd, IDC_EDIT_STR, EM_SETSEL, 0, -1);
			// WM_COMMAND を発生させて以前の文字列を消してしまうので最後に設定
			SendMessage(hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hWnd, IDC_EDIT_STR), TRUE);
			break;

		case WM_COMMAND:
			dedit_get_type(lpData, hWnd);
			dedit_str_to_bytes(lpData, hWnd, IDC_EDIT_STR);
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hWnd, (INT_PTR)lpData);
					return TRUE;

				case IDCANCEL:
					EndDialog(hWnd, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

static INT_PTR CALLBACK dedit_edit_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_INITDIALOG)
	{
		SetDlgItemText(hWnd, IDC_EDIT_GROUP, _T("Edit"));
	}
	return dedit_base_proc(&s_editData, hWnd, msg, wParam, lParam);
}

static INT_PTR CALLBACK dedit_find_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_INITDIALOG)
	{
		SetWindowText(hWnd, _T("Find..."));
		SetDlgItemText(hWnd, IDC_EDIT_GROUP, _T("Find"));
	}
	return dedit_base_proc(&s_findData, hWnd, msg, wParam, lParam);
}

EDITDATA* dedit_input(HWND hParent, BOOL bFind)
{
	HINSTANCE hInstance = CWndProc::GetResourceHandle();
	INT_PTR ret = DialogBox(hInstance, MAKEINTRESOURCE(IDD_EDIT), hParent,
		bFind ? dedit_find_proc : dedit_edit_proc);
	return (ret != 0) ? (EDITDATA*)ret : NULL;
}

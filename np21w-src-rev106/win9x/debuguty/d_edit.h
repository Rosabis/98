/**
 * @file	d_edit.h
 * @brief	メモリ編�? / 検索ダイアログ (np2debug より移�?)
 */

#pragma once

#include "compiler.h"

#define	MAX_EDIT_STR	256

/// 編�?�?ータ
struct tagEditData
{
	UINT8	bytes[MAX_EDIT_STR * 3];
	TCHAR	str[MAX_EDIT_STR];
	int		bytes_len;
	UINT16	type;
	DWORD	codepage;
};
typedef struct tagEditData			EDITDATA;
typedef struct tagEditData			*PEDITDATA;
typedef const struct tagEditData	*PCEDITDATA;

/// メモリ編�? / 検索ダイアログ
/// --------------------
// ダイアログを開き、OK なら�?��?部バッファへのポインタ、キャンセルな�? NULL�?
//!
//! @param[in] hParent 親ウィンドウ
//! @param[in] bFind   TRUE: 検索ダイアログ / FALSE: 編�?ダイアログ
//! @return OK 時�?� EDITDATA*、キャンセル時�?� NULL
EDITDATA* dedit_input(HWND hParent, BOOL bFind);
/// --------------------

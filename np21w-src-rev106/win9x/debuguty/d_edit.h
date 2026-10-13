/**
 * @file	d_edit.h
 * @brief	メモリ編集 / 検索ダイアログ (np2debug より移植)
 */

#pragma once

#include "compiler.h"

#define	MAX_EDIT_STR	256

/// 編集データ
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

/// メモリ編集 / 検索ダイアログ
/// --------------------
// ダイアログを開き、OK ならば内部バッファへのポインタ、キャンセルなら NULL。
//!
//! @param[in] hParent 親ウィンドウ
//! @param[in] bFind   TRUE: 検索ダイアログ / FALSE: 編集ダイアログ
//! @return OK 時は EDITDATA*、キャンセル時は NULL
EDITDATA* dedit_input(HWND hParent, BOOL bFind);
/// --------------------

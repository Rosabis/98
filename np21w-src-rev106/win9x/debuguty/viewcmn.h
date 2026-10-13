/**
 * @file	viewcmn.h
 * @brief	DebugUty ビュー共通処理の宣言ヘッダー (np2debug より移植)
 */

#pragma once

#include "compiler.h"

/// ブレークポイント関連
/// --------------------
// 物理アドレスにブレークポイントが設定されているか
BOOL viewcmn_has_breakpoint(UINT32 addr);

// ブレークポイントの種類を表す文字 ('R' / 'W' / 'X' / ' ')
TCHAR viewcmn_breakpoint_char(UINT32 addr);

// 指定アドレスのブレークポイントを反転させる
void viewcmn_toggle_breakpoint(UINT32 addr);
/// --------------------

/// 表示補助
/// --------
// 物理アドレスを "SEG:OFF" または "XXXXXXXX" 形式で出力する
void viewcmn_print_addr(LPTSTR lpString, UINT32 seg, UINT32 addr, BOOL bSegmented);

// 16 進数文字列を解析する。失敗すれば FALSE。
BOOL viewcmn_parse_hex(LPCTSTR lpString, UINT32* lpValue);
/// --------

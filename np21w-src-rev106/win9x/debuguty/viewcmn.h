/**
 * @file	viewcmn.h
 * @brief	DebugUty ビュー共通�?��?の宣言ヘッダー (np2debug より移�?)
 */

#pragma once

#include "compiler.h"

/// ブレークポイント関連
/// --------------------
// 物�?アドレスにブレークポイントが設定されて�?るか
BOOL viewcmn_has_breakpoint(UINT32 addr);

// ブレークポイント�?�種類を表す文�? ('R' / 'W' / 'X' / ' ')
TCHAR viewcmn_breakpoint_char(UINT32 addr);

// �?定アドレスのブレークポイントを反転させ�?
void viewcmn_toggle_breakpoint(UINT32 addr);
/// --------------------

/// 表示補助
/// --------
// 物�?アドレス�? "SEG:OFF" また�?� "XXXXXXXX" 形式で出力す�?
void viewcmn_print_addr(LPTSTR lpString, UINT32 seg, UINT32 addr, BOOL bSegmented);

// 16 進数�?字�?�を解析する。失敗すれ�?� FALSE�?
BOOL viewcmn_parse_hex(LPCTSTR lpString, UINT32* lpValue);
/// --------

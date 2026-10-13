/**
 * @file	viewmenu.h
 * @brief	DebugUty ビューメニュー処理の宣言ヘッダー (np2debug より移植)
 */

#pragma once

class CDebugUtyView;

/// メニュー状態更新
/// ----------------
// モードのラジオチェック
void viewmenu_mode(CDebugUtyView* lpView);

// ロックのチェック
void viewmenu_lock(CDebugUtyView* lpView);

// デバッグメニューの有効 / 無効 (bRunning == 実行中)
void viewmenu_debug_toggle(CDebugUtyView* lpView, BOOL bRunning);
/// ----------------

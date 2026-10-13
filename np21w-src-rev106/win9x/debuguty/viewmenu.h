/**
 * @file	viewmenu.h
 * @brief	DebugUty ビューメニュー処�?の宣言ヘッダー (np2debug より移�?)
 */

#pragma once

class CDebugUtyView;

/// メニュー状態更新
/// ----------------
// モード�?�ラジオチェ�?ク
void viewmenu_mode(CDebugUtyView* lpView);

// ロ�?クのチェ�?ク
void viewmenu_lock(CDebugUtyView* lpView);

// �?バッグメニューの有効 / 無効 (bRunning == 実行中)
void viewmenu_debug_toggle(CDebugUtyView* lpView, BOOL bRunning);
/// ----------------

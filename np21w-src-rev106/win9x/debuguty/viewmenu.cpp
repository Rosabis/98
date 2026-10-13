/**
 * @file	viewmenu.cpp
 * @brief	DebugUty ビューメニュー処�?の実�? (np2debug より移�?)
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewmenu.h"
#include "viewer.h"
#include "viewitem.h"

#define	MFCHECK(a)	((a) ? MF_CHECKED : MF_UNCHECKED)

/**
 * モード�?�ラジオチェ�?ク
 * @param[in] lpView ビュー
 */
void viewmenu_mode(CDebugUtyView* lpView)
{
	HMENU hMenu = lpView->GetMenu();
	if (hMenu == NULL)
	{
		return;
	}
	const UINT nID = lpView->GetItemID();
	::CheckMenuRadioItem(hMenu, IDM_VIEWMODEREG, IDM_VIEWMODESTK, nID, MF_BYCOMMAND);
	::CheckMenuItem(hMenu, IDM_VIEWMODELOCK, MF_BYCOMMAND | MFCHECK(lpView->IsItemLocked()));
}

/**
 * ロ�?クのチェ�?ク
 * @param[in] lpView ビュー
 */
void viewmenu_lock(CDebugUtyView* lpView)
{
	HMENU hMenu = lpView->GetMenu();
	if (hMenu == NULL)
	{
		return;
	}
	::CheckMenuItem(hMenu, IDM_VIEWMODELOCK, MF_BYCOMMAND | MFCHECK(lpView->IsItemLocked()));
}

/**
 * �?バッグメニューの有効 / 無効
 * @param[in] lpView ビュー
 * @param[in] bRunning 実行中�?
 */
void viewmenu_debug_toggle(CDebugUtyView* lpView, BOOL bRunning)
{
	HMENU hMenu = lpView->GetMenu();
	if (hMenu == NULL)
	{
		return;
	}
	DWORD flag = bRunning ? MF_GRAYED : MF_ENABLED;
	::EnableMenuItem(hMenu, IDM_DEBUG_RUN, MF_BYCOMMAND | flag);
	::EnableMenuItem(hMenu, IDM_DEBUG_STEPINTO, MF_BYCOMMAND | flag);
	::EnableMenuItem(hMenu, IDM_DEBUG_STEPOVER, MF_BYCOMMAND | flag);

	flag = bRunning ? MF_ENABLED : MF_GRAYED;
	::EnableMenuItem(hMenu, IDM_DEBUG_STOP, MF_BYCOMMAND | flag);
}

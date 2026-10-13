/**
 * @file	viewer.h
 * @brief	DebugUty ?ｿｽp?ｿｽr?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽN?ｿｽ?ｿｽ?ｿｽX?ｿｽﾌ宣言?ｿｽ?ｿｽ?ｿｽ?ｿｽﾑイ?ｿｽ?ｿｽ?ｿｽ^?ｿｽ[?ｿｽt?ｿｽF?ｿｽC?ｿｽX?ｿｽﾌ抵ｿｽ`?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 */

#pragma once

#include "..\misc\WndProc.h"

class CDebugUtyItem;

//! ?ｿｽr?ｿｽ?ｿｽ?ｿｽ[?ｿｽﾅ大数
#define NP2VIEW_MAX		8

/**
 * @brief ?ｿｽr?ｿｽ?ｿｽ?ｿｽ[ ?ｿｽN?ｿｽ?ｿｽ?ｿｽX
 */
class CDebugUtyView : public CWndProc
{
public:
	static void Initialize(HINSTANCE hInstance);
	static void New();
	static void DisposeAllClosedWindow();
	static void AllClose();
	static void AllUpdate(bool bForce);
	static void AllMenuState(bool bRunning);

	CDebugUtyView();
	virtual ~CDebugUtyView();
	void DetachDebugView();
	void UpdateCaption();
	UINT32 GetVScrollPos() const;
	void SetVScrollPos(UINT nPos);
	void SetVScroll(UINT nPos, UINT nLines);
	void UpdateVScroll();

	UINT GetItemID() const;
	bool IsItemLocked() const;

	//! 繧ｹ繝?繝ｼ繧ｿ繧ｹ繝舌?ｼ (viewstat)
	void OpenStatusBar();
	void CloseStatusBar();
	void UpdateStatusBar();
	void SetStatusMessage(LPCTSTR lpText);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT nType, int cx, int cy);
	void OnPaint();
	void OnVScroll(UINT nSBCode, UINT nPos, HWND hwndScrollBar);
	void OnEnterMenuLoop(BOOL bIsTrackPopupMenu);
	void OnActivate(UINT nState, HWND hwndOther, BOOL bMinimized);
	virtual void PostNcDestroy();

private:
	bool m_bActive;				//!< ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu ?ｿｽt?ｿｽ?ｿｽ?ｿｽO
	UINT m_nVPos;				//!< ?ｿｽﾊ置
	UINT m_nVLines;				//!< ?ｿｽ?ｿｽ?ｿｽC?ｿｽ?ｿｽ?ｿｽ?ｿｽ
	UINT m_nVPage;				//!< 1?ｿｽy?ｿｽ[?ｿｽW?ｿｽﾌ表?ｿｽ?ｿｽ?ｿｽ?ｿｽ
	UINT m_nVMultiple;			//!< ?ｿｽ{?ｿｽ?ｿｽ
	CDebugUtyItem* m_lpItem;	//!< ?ｿｽ\?ｿｽ?ｿｽ?ｿｽA?ｿｽC?ｿｽe?ｿｽ?ｿｽ
	HWND m_hWndStatus;		//!< ?ｿｽX?ｿｽe?ｿｽb?ｿｽ[?ｿｽ^?ｿｽ[ (viewstat)
	static DWORD sm_dwLastTick;	//!< ?ｿｽﾅ鯉ｿｽ?ｿｽTick
	void SetMode(UINT nID);
	void SetSegmentItem(HMENU hMenu, int nId, LPCTSTR lpSegment, UINT nSegment);
	void UpdateView();
	static void UpdateActive();
};

/**
 * ?ｿｽ?ｿｽ?ｿｽﾝの位置?ｿｽ?ｿｽﾔゑｿｽ
 * @return ?ｿｽﾊ置
 */
inline UINT32 CDebugUtyView::GetVScrollPos() const
{
	return m_nVPos;
}

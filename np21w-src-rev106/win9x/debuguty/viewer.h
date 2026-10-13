/**
 * @file	viewer.h
 * @brief	DebugUty �p�r���[�� �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂�
 */

#pragma once

#include "..\misc\WndProc.h"

class CDebugUtyItem;

//! �r���[�ő吔
#define NP2VIEW_MAX		8

/**
 * @brief �r���[ �N���X
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

	//! ステータスバー (viewstat)
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
	bool m_bActive;				//!< �A�N�e�B�u �t���O
	UINT m_nVPos;				//!< �ʒu
	UINT m_nVLines;				//!< ���C����
	UINT m_nVPage;				//!< 1�y�[�W�̕\����
	UINT m_nVMultiple;			//!< �{��
	CDebugUtyItem* m_lpItem;	//!< �\���A�C�e��
	HWND m_hWndStatus;		//!< �X�e�b�[�^�[ (viewstat)
	static DWORD sm_dwLastTick;	//!< �Ō��Tick
	void SetMode(UINT nID);
	void SetSegmentItem(HMENU hMenu, int nId, LPCTSTR lpSegment, UINT nSegment);
	void UpdateView();
	static void UpdateActive();
};

/**
 * ���݂̈ʒu��Ԃ�
 * @return �ʒu
 */
inline UINT32 CDebugUtyView::GetVScrollPos() const
{
	return m_nVPos;
}

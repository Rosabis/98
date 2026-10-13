/**
 * @file	viewstk.h
 * @brief	スタック ビュークラスの宣言ヘッダー (np2debug より移植)
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief スタック ビュークラス
 */
class CDebugUtyStk : public CDebugUtyItem
{
public:
	CDebugUtyStk(CDebugUtyView* lpView);
	virtual ~CDebugUtyStk();

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual UINT32 GetAddress() const;
	virtual UINT GetSegment() const;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC, const RECT& rect);

private:
	UINT m_nSegment;							//!< セグメント
	UINT m_nOffset;								//!< オフセット
	DebugUtyViewMemory m_mem;					//!< メモリ
	std::vector<unsigned char> m_buffer;		//!< バッファ
	void SetStack();
};

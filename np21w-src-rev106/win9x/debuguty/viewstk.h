/**
 * @file	viewstk.h
 * @brief	スタ�?ク ビュークラスの宣言ヘッダー (np2debug より移�?)
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief スタ�?ク ビュークラス
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
	virtual DebugUtyViewMemory* GetMemoryConfig() { return &m_mem; }

private:
	UINT m_nSegment;							//!< セグメン�?
	UINT m_nOffset;								//!< オフセ�?�?
	DebugUtyViewMemory m_mem;					//!< メモリ
	std::vector<unsigned char> m_buffer;		//!< バッファ
	void SetStack();
};

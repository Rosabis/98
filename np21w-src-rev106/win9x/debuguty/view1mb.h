/**
 * @file	view1mb.h
 * @brief	?øΩ?øΩ?øΩC?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ ?øΩ?øΩ?øΩW?øΩX?øΩ^?øΩ\?øΩ?øΩ?øΩN?øΩ?øΩ?øΩX?øΩÃêÈåæ?øΩ?øΩ?øΩ?øΩ—ÉC?øΩ?øΩ?øΩ^?øΩ[?øΩt?øΩF?øΩC?øΩX?øΩÃíÔøΩ`?øΩ?øΩ?øΩ?øΩ?øΩ‹ÇÔøΩ
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ ?øΩ?øΩ?øΩW?øΩX?øΩ^?øΩ\?øΩ?øΩ?øΩN?øΩ?øΩ?øΩX
 */
class CDebugUty1MB : public CDebugUtyItem
{
public:
	CDebugUty1MB(CDebugUtyView* lpView);
	virtual ~CDebugUty1MB();

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual UINT32 GetAddress() const;
	virtual UINT GetSegment() const;
	virtual void SetAddress(UINT32 addr);
	virtual void ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC, const RECT& rect);

private:
	DebugUtyViewMemory m_mem;					//!< ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
	std::vector<unsigned char> m_buffer;		//!< ?øΩo?øΩb?øΩt?øΩ@
	void SetSegment(UINT nSegment);
};

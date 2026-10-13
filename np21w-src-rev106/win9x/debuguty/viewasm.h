/**
 * @file	viewasm.h
 * @brief	?øΩA?øΩZ?øΩ?øΩ?øΩu?øΩ?øΩ ?øΩ?øΩ?øΩX?øΩg?øΩ\?øΩ?øΩ?øΩN?øΩ?øΩ?øΩX?øΩÃêÈåæ?øΩ?øΩ?øΩ?øΩ—ÉC?øΩ?øΩ?øΩ^?øΩ[?øΩt?øΩF?øΩC?øΩX?øΩÃíÔøΩ`?øΩ?øΩ?øΩ?øΩ?øΩ‹ÇÔøΩ
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief ?øΩA?øΩZ?øΩ?øΩ?øΩu?øΩ?øΩ ?øΩ?øΩ?øΩX?øΩg?øΩ\?øΩ?øΩ?øΩN?øΩ?øΩ?øΩX
 */
class CDebugUtyAsm : public CDebugUtyItem
{
public:
	CDebugUtyAsm(CDebugUtyView* lpView);
	virtual ~CDebugUtyAsm();

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual UINT32 GetAddress() const;
	virtual UINT GetSegment() const;
	virtual void SetAddress(UINT32 addr);
	virtual void ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer);
	virtual void OnPaint(HDC hDC, const RECT& rect);

private:
	UINT m_nSegment;							//!< ?øΩZ?øΩO?øΩ?øΩ?øΩ?øΩ?øΩg
	UINT m_nOffset;								//!< ?øΩZ?øΩO?øΩ?øΩ?øΩ?øΩ?øΩg
	DebugUtyViewMemory m_mem;					//!< ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
	std::vector<unsigned char> m_buffer;		//!< ?øΩo?øΩb?øΩt?øΩ@
	std::vector<UINT> m_address;				//!< ?øΩA?øΩh?øΩ?øΩ?øΩX ?øΩo?øΩb?øΩt?øΩ@
	void ReadMemory(UINT nOffset, unsigned char* lpBuffer, UINT cbBuffer) const;
};

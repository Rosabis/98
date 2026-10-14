/**
 * @file	viewitem.h
 * @brief	DebugUty ?¿½p?¿½r?¿½?¿½?¿½[?¿½?¿½?¿½N?¿½?¿½?¿½X?¿½ÌéŒ¾?¿½?¿½?¿½?¿½ÑƒC?¿½?¿½?¿½^?¿½[?¿½t?¿½F?¿½C?¿½X?¿½Ì’ï¿½`?¿½?¿½?¿½?¿½?¿½Ü‚ï¿½
 */

#pragma once

#include "viewmem.h"

class CDebugUtyView;

/**
 * @brief ?¿½f?¿½o?¿½O?¿½\?¿½?¿½?¿½A?¿½C?¿½e?¿½?¿½?¿½ÌŠï¿½?¿½N?¿½?¿½?¿½X
 */
class CDebugUtyItem
{
public:
	static CDebugUtyItem* New(UINT nID, CDebugUtyView* lpView, const CDebugUtyItem* lpItem = NULL);

	CDebugUtyItem(CDebugUtyView* lpView, UINT nID);
	virtual ~CDebugUtyItem();
	UINT GetID() const;

	//! ç¾åœ¨ã‚«ãƒ¼ã‚½ãƒ«ãŒã‚ã‚‹ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ (ã‚¹ãƒ?ãƒ¼ã‚¿ã‚¹ãƒã?¼ / ãƒ–ãƒ¬ãƒ¼ã‚¯ãƒã‚¤ãƒ³ãƒˆç”¨)
	virtual UINT32 GetAddress() const;
	//! ç¾åœ¨ã®ã‚»ã‚°ãƒ¡ãƒ³ãƒ?
	virtual UINT GetSegment() const;

	//! ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã¸ç§»å‹•ã™ã‚? (d_addr ãƒ€ã‚¤ã‚¢ãƒ­ã‚° / æ¤œç´¢çµæœ)
	virtual void SetAddress(UINT32 addr);

	//! ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã‹ã‚‰èª­ã¿å‡ºã? (ãƒ“ãƒ¥ãƒ¼ã”ã¨ã®ãƒ¡ãƒ¢ãƒªè¨­å®šã‚’åæ˜ )
	virtual void ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer);

	//! ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã¸æ›¸ãè¾¼ã‚€ (d_edit ãƒ€ã‚¤ã‚¢ãƒ­ã‚°)
	virtual void WriteMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes);

	//! ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ä»¥é™ã‹ã‚‰ãƒã‚¤ãƒˆå?—ã‚’æ¤œç´¢ã™ã‚‹
	virtual BOOL FindMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes, UINT32* lpFound);

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC, const RECT& rect);

	//! memory bank config accessor (NULL when the view has no memory settings)
	virtual DebugUtyViewMemory* GetMemoryConfig() { return NULL; }

protected:
	CDebugUtyView* m_lpView;	//!< ?¿½r?¿½?¿½?¿½[ ?¿½N?¿½?¿½?¿½X
	UINT m_nID;					//!< ID
};

/**
 * ID ?¿½?¿½Ô‚ï¿½
 * @return ID
 */
inline UINT CDebugUtyItem::GetID() const
{
	return m_nID;
}

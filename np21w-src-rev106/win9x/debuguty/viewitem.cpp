/**
 * @file	viewitem.cpp
 * @brief	DebugUty ?øΩp?øΩr?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩN?øΩ?øΩ?øΩX?øΩÃìÔøΩ?øΩ?øΩÃíÔøΩ`?øΩ?øΩ?øΩs?øΩ?øΩ?øΩ‹ÇÔøΩ
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewitem.h"
#include "viewer.h"
#include "viewmem.h"
#include "viewreg.h"
#include "viewseg.h"
#include "viewstk.h"
#include "view1mb.h"
#include "viewasm.h"
#include "viewsnd.h"
#include "cpucore.h"
#include "cpumem.h"

#include <string.h>

/**
 * ?øΩC?øΩ?øΩ?øΩX?øΩ^?øΩ?øΩ?øΩX?øΩ?ê¨
 * @param[in] nID ?øΩr?øΩ?øΩ?øΩ[ ID
 * @param[in] lpView ?øΩr?øΩ?øΩ?øΩ[?øΩ?øΩ ?øΩC?øΩ?øΩ?øΩX?øΩ^?øΩ?øΩ?øΩX
 * @param[in] lpItem ?øΩÓèÄ?øΩ∆Ç»ÇÔøΩA?øΩC?øΩe?øΩ?øΩ
 * @return ?øΩC?øΩ?øΩ?øΩX?øΩ^?øΩ?øΩ?øΩX
 */
CDebugUtyItem* CDebugUtyItem::New(UINT nID, CDebugUtyView* lpView, const CDebugUtyItem* lpItem)
{
	CDebugUtyItem* lpNewItem = NULL;
	switch (nID)
	{
		case IDM_VIEWMODEREG:
			lpNewItem = new CDebugUtyReg(lpView);
			break;

		case IDM_VIEWMODESEG:
			lpNewItem = new CDebugUtySeg(lpView);
			break;

		case IDM_VIEWMODESTK:
			lpNewItem = new CDebugUtyStk(lpView);
			break;

		case IDM_VIEWMODE1MB:
			lpNewItem = new CDebugUty1MB(lpView);
			break;

		case IDM_VIEWMODEASM:
			lpNewItem = new CDebugUtyAsm(lpView);
			break;

		case IDM_VIEWMODESND:
			lpNewItem = new CDebugUtySnd(lpView);
			break;
	}
	if (lpNewItem == NULL)
	{
		lpNewItem = new CDebugUtyItem(lpView, nID);
	}
	lpNewItem->Initialize(lpItem);
	return lpNewItem;
}

/**
 * ?øΩR?øΩ?øΩ?øΩX?øΩg?øΩ?øΩ?øΩN?øΩ^
 * @param[in] lpView ?øΩr?øΩ?øΩ?øΩ[?øΩ?øΩ ?øΩC?øΩ?øΩ?øΩX?øΩ^?øΩ?øΩ?øΩX
 * @param[in] nID ?øΩr?øΩ?øΩ?øΩ[ ID
 */
CDebugUtyItem::CDebugUtyItem(CDebugUtyView* lpView, UINT nID)
	: m_lpView(lpView)
	, m_nID(nID)
{
}

/**
 * ?øΩf?øΩX?øΩg?øΩ?øΩ?øΩN?øΩ^
 */
CDebugUtyItem::~CDebugUtyItem()
{
}

/**
 * ÁèæÂú®„Ç´„Éº„ÇΩ„É´„Åå„ÅÇ„ÇãÁâ©Áê?„Ç¢„Éâ„É¨„Çπ„ÇíËøî„Åô
 * @return Áâ©Áê?„Ç¢„Éâ„É¨„Çπ
 */
UINT32 CDebugUtyItem::GetAddress() const
{
	return 0;
}

/**
 * ÁèæÂú®„ÅÆ„Çª„Ç∞„É°„É≥„Éà„ÇíËøî„Åô
 * @return „Çª„Ç∞„É°„É≥„É?
 */
UINT CDebugUtyItem::GetSegment() const
{
	return 0;
}

/**
 * Áâ©Áê?„Ç¢„Éâ„É¨„Çπ„Å∏ÁßªÂãï„Åô„Ç?
 * @param[in] addr Áâ©Áê?„Ç¢„Éâ„É¨„Çπ
 */
void CDebugUtyItem::SetAddress(UINT32 addr)
{
	// Êó¢ÂÆö„Åß„ÅØ‰Ωï„ÇÇ„Åó„Å™„Å?
}

/**
 * Áâ©Áê?„Ç¢„Éâ„É¨„Çπ„Åã„ÇâË™≠„ÅøÂá∫„Å?
 * @param[in] nAddress Áâ©Áê?„Ç¢„Éâ„É¨„Çπ
 * @param[in] lpBuffer „Éê„ÉÉ„Éï„Ç°
 * @param[in] cbBuffer „Éê„ÉÉ„Éï„Ç°„Çµ„Ç§„Ç∫
 */
void CDebugUtyItem::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
{
	memp_reads(nAddress, lpBuffer, cbBuffer);
}

/**
 * Áâ©Áê?„Ç¢„Éâ„É¨„Çπ„Å∏Êõ∏„ÅçËæº„ÇÄ
 * @param[in] nAddress Áâ©Áê?„Ç¢„Éâ„É¨„Çπ
 * @param[in] lpBytes „Éê„Ç§„ÉàÂ??
 * @param[in] cbBytes „Éê„Ç§„ÉàÊï∞
 */
void CDebugUtyItem::WriteMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes)
{
	memp_writes(nAddress, lpBytes, cbBytes);
}

/**
 * Áâ©Áê?„Ç¢„Éâ„É¨„Çπ‰ª•Èôç„Åã„Çâ„Éê„Ç§„ÉàÂ?ó„ÇíÊ§úÁ¥¢„Åô„Çã
 * @param[in] nAddress Ê§úÁ¥¢ÈñãÂßãÁâ©Áê?„Ç¢„Éâ„É¨„Çπ
 * @param[in] lpBytes „Éê„Ç§„ÉàÂ??
 * @param[in] cbBytes „Éê„Ç§„ÉàÊï∞
 * @param[out] lpFound Ë¶ã„Å§„Åã„Å£„ÅüÁâ©Áê?„Ç¢„Éâ„É¨„Çπ
 * @retval true Ë¶ã„Å§„Åã„Å£„Å?
 * @retval false Ë¶ã„Å§„Åã„Çâ„Å™„Åã„Å£„Å?
 */
BOOL CDebugUtyItem::FindMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes, UINT32* lpFound)
{
	UINT8 buf[64];

	if ((lpBytes == NULL) || (cbBytes == 0) || (cbBytes > sizeof(buf)))
	{
		return FALSE;
	}
	if ((nAddress + cbBytes) > 0x110000)
	{
		return FALSE;
	}
	for (UINT32 a = nAddress; (a + cbBytes) <= 0x110000; a++)
	{
		ReadMemory(a, buf, cbBytes);
		if (memcmp(buf, lpBytes, cbBytes) == 0)
		{
			if (lpFound)
			{
				*lpFound = a;
			}
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
 * @param[in] lpItem ?øΩ?øΩ?øΩt?øΩ@?øΩ?øΩ?øΩ?øΩ?øΩX
 */
void CDebugUtyItem::Initialize(const CDebugUtyItem* lpItem)
{
	m_lpView->SetVScroll(0, 0);
}

/**
 * ?øΩX?øΩV
 * @retval true ?øΩX?øΩV?øΩ?øΩ?øΩ?øΩ
 * @retval false ?øΩX?øΩV?øΩ»ÇÔøΩ
 */
bool CDebugUtyItem::Update()
{
	return false;
}

/**
 * ?øΩ?øΩ?øΩb?øΩN
 * @retval true ?øΩ?øΩ?øΩ?øΩ
 * @retval false ?øΩ?øΩ?øΩs
 */
bool CDebugUtyItem::Lock()
{
	return false;
}

/**
 * ?øΩA?øΩ?øΩ?øΩ?øΩ?øΩb?øΩN
 */
void CDebugUtyItem::Unlock()
{
}

/**
 * ?øΩ?øΩ?øΩb?øΩN?øΩ?øΩ?
 * @retval true ?øΩ?øΩ?øΩb?øΩN?øΩ?øΩ?øΩ≈ÇÔøΩ?øΩ?øΩ
 * @retval false ?øΩ?øΩ?øΩb?øΩN?øΩ?øΩ?øΩ≈Ç»ÇÔøΩ
 */
bool CDebugUtyItem::IsLocked()
{
	return false;
}

/**
 * ?øΩ?øΩ?øΩ[?øΩU?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩj?øΩ?øΩ?øΩ[?øΩÃçÔøΩ?øΩ⁄ÇÔøΩI?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ∆ÇÔøΩ?øΩ…ÅA?øΩt?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩN?øΩ…ÇÔøΩ?øΩ?øΩƒåƒÇ—èo?øΩ?øΩ?øΩ?øΩ‹ÇÔøΩ
 * @param[in] wParam ?øΩp?øΩ?øΩ?øΩ?øΩ?øΩ^
 * @param[in] lParam ?øΩp?øΩ?øΩ?øΩ?øΩ?øΩ^
 * @retval TRUE ?øΩA?øΩv?øΩ?øΩ?øΩP?øΩ[?øΩV?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃ??øΩ?øΩb?øΩZ?øΩ[?øΩW?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
 * @retval FALSE ?øΩA?øΩv?øΩ?øΩ?øΩP?øΩ[?øΩV?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩÃ??øΩ?øΩb?øΩZ?øΩ[?øΩW?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ»ÇÔøΩ?øΩ?øΩ?øΩ?øΩ
 */
BOOL CDebugUtyItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

/**
 * ?øΩ`?øΩ?øΩ
 * @param[in] hDC ?øΩf?øΩo?øΩC?øΩX ?øΩR?øΩ?øΩ?øΩe?øΩL?øΩX?øΩg
 * @param[in] rect ?øΩÃàÔøΩ
 */
void CDebugUtyItem::OnPaint(HDC hDC, const RECT& rect)
{
}

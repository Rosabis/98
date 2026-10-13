/**
 * @file	viewreg.cpp
 * @brief	?¿½?¿½?¿½?¿½?¿½?¿½?¿½\?¿½?¿½?¿½N?¿½?¿½?¿½X?¿½Ì“ï¿½?¿½?¿½Ì’ï¿½`?¿½?¿½?¿½s?¿½?¿½?¿½Ü‚ï¿½
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewseg.h"
#include "viewer.h"
#include "viewcmn.h"
#include "cpucore.h"

/**
 * ?¿½R?¿½?¿½?¿½X?¿½g?¿½?¿½?¿½N?¿½^
 * @param[in] lpView ?¿½r?¿½?¿½?¿½[?¿½?¿½ ?¿½C?¿½?¿½?¿½X?¿½^?¿½?¿½?¿½X
 */
CDebugUtySeg::CDebugUtySeg(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODESEG)
	, m_nSegment(0)
{
}

/**
 * ?¿½f?¿½X?¿½g?¿½?¿½?¿½N?¿½^
 */
CDebugUtySeg::~CDebugUtySeg()
{
}

/**
 * ?¿½?¿½?¿½?¿½?¿½?¿½
 * @param[in] lpItem ?¿½î€?¿½Æ‚È‚ï¿½A?¿½C?¿½e?¿½?¿½
 */
void CDebugUtySeg::Initialize(const CDebugUtyItem* lpItem)
{
	m_lpView->SetVScroll(0, 0x1000);
}

/**
 * ?¿½X?¿½V
 * @retval true ?¿½X?¿½V?¿½?¿½?¿½?¿½
 * @retval false ?¿½X?¿½V?¿½È‚ï¿½
 */
bool CDebugUtySeg::Update()
{
	if (!m_buffer.empty())
	{
		return false;
	}
	m_mem.Update();
	return true;
}

/**
 * ?¿½?¿½?¿½b?¿½N
 * @retval true ?¿½?¿½?¿½?¿½
 * @retval false ?¿½?¿½?¿½s
 */
bool CDebugUtySeg::Lock()
{
	m_buffer.resize(0x10000);

	m_mem.Update();
	m_mem.Read(m_nSegment << 4, &m_buffer.at(0), static_cast<UINT>(m_buffer.size()));
	return true;
}

/**
 * ?¿½A?¿½?¿½?¿½?¿½?¿½b?¿½N
 */
void CDebugUtySeg::Unlock()
{
	m_buffer.clear();
}

/**
 * ?¿½?¿½?¿½b?¿½N?¿½?¿½?
 * @retval true ?¿½?¿½?¿½b?¿½N?¿½?¿½?¿½Å‚ï¿½?¿½?¿½
 * @retval false ?¿½?¿½?¿½b?¿½N?¿½?¿½?¿½Å‚È‚ï¿½
 */
bool CDebugUtySeg::IsLocked()
{
	return (!m_buffer.empty());
}

/**
 * ç¾åœ¨ã‚«ãƒ¼ã‚½ãƒ«ãŒã‚ã‚‹ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã‚’è¿”ã™
 * @return ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹
 */
UINT32 CDebugUtySeg::GetAddress() const
{
	return (m_nSegment + m_lpView->GetVScrollPos()) << 4;
}

/**
 * ç¾åœ¨ã®ã‚»ã‚°ãƒ¡ãƒ³ãƒˆã‚’è¿”ã™
 * @return ã‚»ã‚°ãƒ¡ãƒ³ãƒ?
 */
UINT CDebugUtySeg::GetSegment() const
{
	return m_nSegment;
}

/**
 * ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã¸ç§»å‹•ã™ã‚?
 * @param[in] addr ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹
 */
void CDebugUtySeg::SetAddress(UINT32 addr)
{
	// ã‚¹ã‚¯ãƒ­ãƒ¼ãƒ«ä½ç½®ã¯ã‚»ã‚°ãƒ¡ãƒ³ãƒˆå??ã® 16 ãƒã‚¤ãƒˆå˜ä½?
	SetSegment(static_cast<UINT>((addr >> 4) & 0xf000));
	m_lpView->SetVScrollPos(static_cast<UINT>((addr >> 4) & 0x0fff));
}

/**
 * ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹ã‹ã‚‰èª­ã¿å‡ºã?
 * @param[in] nAddress ç‰©ç?ã‚¢ãƒ‰ãƒ¬ã‚¹
 * @param[in] lpBuffer ãƒãƒƒãƒ•ã‚¡
 * @param[in] cbBuffer ãƒãƒƒãƒ•ã‚¡ã‚µã‚¤ã‚º
 */
void CDebugUtySeg::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
{
	m_mem.Read(nAddress, lpBuffer, cbBuffer);
}

/**
 * ?¿½?¿½?¿½[?¿½U?¿½[?¿½?¿½?¿½?¿½?¿½j?¿½?¿½?¿½[?¿½Ìï¿½?¿½Ú‚ï¿½I?¿½?¿½?¿½?¿½?¿½?¿½?¿½Æ‚ï¿½?¿½ÉA?¿½t?¿½?¿½?¿½[?¿½?¿½?¿½?¿½?¿½[?¿½N?¿½É‚ï¿½?¿½?¿½ÄŒÄ‚Ño?¿½?¿½?¿½?¿½Ü‚ï¿½
 * @param[in] wParam ?¿½p?¿½?¿½?¿½?¿½?¿½^
 * @param[in] lParam ?¿½p?¿½?¿½?¿½?¿½?¿½^
 * @retval TRUE ?¿½A?¿½v?¿½?¿½?¿½P?¿½[?¿½V?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½Ì??¿½?¿½b?¿½Z?¿½[?¿½W?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½
 * @retval FALSE ?¿½A?¿½v?¿½?¿½?¿½P?¿½[?¿½V?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½Ì??¿½?¿½b?¿½Z?¿½[?¿½W?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½?¿½È‚ï¿½?¿½?¿½?¿½?¿½
 */
BOOL CDebugUtySeg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case IDM_SEGCS:
			SetSegment(CPU_CS);
			break;

		case IDM_SEGDS:
			SetSegment(CPU_DS);
			break;

		case IDM_SEGES:
			SetSegment(CPU_ES);
			break;

		case IDM_SEGSS:
			SetSegment(CPU_SS);
			break;

		case IDM_SEGTEXT:
			SetSegment(0xa000);
			break;

		default:
			return FALSE;

	}
	return TRUE;
}

/**
 * ?¿½Z?¿½O?¿½?¿½?¿½?¿½?¿½g?¿½ÏX
 * @param[in] nSegment ?¿½Z?¿½O?¿½?¿½?¿½?¿½?¿½g
 */
void CDebugUtySeg::SetSegment(UINT nSegment)
{
	if (m_nSegment != nSegment)
	{
		m_nSegment = nSegment;
		m_lpView->Invalidate();
	}
}

/**
 * ?¿½`?¿½?¿½
 * @param[in] hDC ?¿½f?¿½o?¿½C?¿½X ?¿½R?¿½?¿½?¿½e?¿½L?¿½X?¿½g
 * @param[in] rect ?¿½Ìˆï¿½
 */
void CDebugUtySeg::OnPaint(HDC hDC, const RECT& rect)
{
	UINT nIndex = m_lpView->GetVScrollPos();
	for (int y = 0; (y < rect.bottom) && (nIndex < 0x1000); y += 16, nIndex++)
	{
		TCHAR szTmp[16];
		::wsprintf(szTmp, _T("%04x:%04x"), m_nSegment, nIndex << 4);
		::TextOut(hDC, 0, y, szTmp, 9);

#if defined(SUPPORT_NP2BREAK)
		const UINT32 nAddress = (m_nSegment + nIndex) << 4;
		if (viewcmn_has_breakpoint(nAddress))
		{
			const TCHAR c = viewcmn_breakpoint_char(nAddress);
			::TextOut(hDC, 9 * 8, y, &c, 1);
		}
#endif

		unsigned char sBuf[16];
		if (!m_buffer.empty())
		{
			CopyMemory(sBuf, &m_buffer.at(nIndex << 4), sizeof(sBuf));
		}
		else
		{
			m_mem.Read((m_nSegment + nIndex) << 4, sBuf, sizeof(sBuf));
		}
		for (int x = 0; x < 16; x++)
		{
			TCHAR szTmp[4];
			::wsprintf(szTmp, TEXT("%02X"), sBuf[x]);
			::TextOut(hDC, ((x * 3) + 11) * 8, y, szTmp, 2);
		}
	}
}

/**
 * @file	viewasm.cpp
 * @brief	??¿½?¿½A??¿½?¿½Z??¿½?¿½??¿½?¿½??¿½?¿½u??¿½?¿½??¿½?¿½ ??¿½?¿½??¿½?¿½??¿½?¿½X??¿½?¿½g??¿½?¿½\??¿½?¿½??¿½?¿½??¿½?¿½N??¿½?¿½??¿½?¿½??¿½?¿½X??¿½?¿½Ì“ï¿½??¿½?¿½??¿½?¿½Ì’ï¿½`??¿½?¿½??¿½?¿½??¿½?¿½s??¿½?¿½??¿½?¿½??¿½?¿½Ü‚ï¿½
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewasm.h"
#include "viewer.h"
#include "viewcmn.h"
#include "unasm.h"
#include "cpucore.h"

/**
 * ??¿½?¿½R??¿½?¿½??¿½?¿½??¿½?¿½X??¿½?¿½g??¿½?¿½??¿½?¿½??¿½?¿½N??¿½?¿½^
 * @param[in] lpView ??¿½?¿½r??¿½?¿½??¿½?¿½??¿½?¿½[??¿½?¿½??¿½?¿½ ??¿½?¿½C??¿½?¿½??¿½?¿½??¿½?¿½X??¿½?¿½^??¿½?¿½??¿½?¿½??¿½?¿½X
 */
CDebugUtyAsm::CDebugUtyAsm(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODEASM)
	, m_nSegment(0)
	, m_nOffset(0)
{
}

/**
 * ??¿½?¿½f??¿½?¿½X??¿½?¿½g??¿½?¿½??¿½?¿½??¿½?¿½N??¿½?¿½^
 */
CDebugUtyAsm::~CDebugUtyAsm()
{
}

/**
 * ??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½
 * @param[in] lpItem ??¿½?¿½î€??¿½?¿½Æ‚È‚ï¿½A??¿½?¿½C??¿½?¿½e??¿½?¿½??¿½?¿½
 */
void CDebugUtyAsm::Initialize(const CDebugUtyItem* lpItem)
{
	m_nSegment = CPU_CS;
	m_nOffset = CPU_IP;
	m_lpView->SetVScroll(0, 0x1000);
}

/**
 * ??¿½?¿½X??¿½?¿½V
 * @retval true ??¿½?¿½X??¿½?¿½V??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½
 * @retval false ??¿½?¿½X??¿½?¿½V??¿½?¿½È‚ï¿½
 */
bool CDebugUtyAsm::Update()
{
	if (!m_buffer.empty())
	{
		return false;
	}

	m_nSegment = CPU_CS;
	m_nOffset = CPU_IP;
	m_lpView->SetVScrollPos(0);
	m_mem.Update();
	m_address.clear();
	return true;
}

/**
 * ??¿½?¿½??¿½?¿½??¿½?¿½b??¿½?¿½N
 * @retval true ??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½
 * @retval false ??¿½?¿½??¿½?¿½??¿½?¿½s
 */
bool CDebugUtyAsm::Lock()
{
	m_buffer.resize(0x10000);
	m_address.clear();

	m_mem.Update();
	m_mem.Read(m_nSegment << 4, &m_buffer.at(0), static_cast<UINT>(m_buffer.size()));
	return true;
}

/**
 * ??¿½?¿½A??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½b??¿½?¿½N
 */
void CDebugUtyAsm::Unlock()
{
	m_buffer.clear();
	m_address.clear();
}

/**
 * ??¿½?¿½??¿½?¿½??¿½?¿½b??¿½?¿½N??¿½?¿½??¿½?¿½?
 * @retval true ??¿½?¿½??¿½?¿½??¿½?¿½b??¿½?¿½N??¿½?¿½??¿½?¿½??¿½?¿½Å‚ï¿½??¿½?¿½??¿½?¿½
 * @retval false ??¿½?¿½??¿½?¿½??¿½?¿½b??¿½?¿½N??¿½?¿½??¿½?¿½??¿½?¿½Å‚È‚ï¿½
 */
bool CDebugUtyAsm::IsLocked()
{
	return (!m_buffer.empty());
}

UINT32 CDebugUtyAsm::GetAddress() const
{
	const UINT nIndex = static_cast<UINT>(m_lpView->GetVScrollPos());
	UINT nOffset = m_nOffset;
	if ((nIndex > 0) && (nIndex <= m_address.size()))
	{
		nOffset = m_address[nIndex - 1];
	}
	return (m_nSegment << 4) + nOffset;
}

UINT CDebugUtyAsm::GetSegment() const
{
	return m_nSegment;
}

/**
 * ç‰©?¿½?ã‚¢ãƒ‰ãƒ¬ã‚¹ã¸ç§»å‹•ã™?¿½?
 * @param[in] addr ç‰©?¿½?ã‚¢ãƒ‰ãƒ¬ã‚¹
 */
void CDebugUtyAsm::SetAddress(UINT32 addr)
{
	m_nSegment = static_cast<UINT>((addr >> 4) & 0xf000);
	m_nOffset = static_cast<UINT>(addr & 0xffff);
	m_address.clear();
	m_lpView->SetVScrollPos(0);
}

/**
 * ç‰©?¿½?ã‚¢ãƒ‰ãƒ¬ã‚¹ã‹ã‚‰èª­ã¿å‡º?¿½?
 * @param[in] nAddress ç‰©?¿½?ã‚¢ãƒ‰ãƒ¬ã‚¹
 * @param[in] lpBuffer ãƒãƒƒãƒ•ã‚¡
 * @param[in] cbBuffer ãƒãƒƒãƒ•ã‚¡ã‚µã‚¤ã‚º
 */
void CDebugUtyAsm::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
{
	m_mem.Read(nAddress, lpBuffer, cbBuffer);
}

/**
 * ??¿½?¿½`??¿½?¿½??¿½?¿½
 * @param[in] hDC ??¿½?¿½f??¿½?¿½o??¿½?¿½C??¿½?¿½X ??¿½?¿½R??¿½?¿½??¿½?¿½??¿½?¿½e??¿½?¿½L??¿½?¿½X??¿½?¿½g
 * @param[in] rect ??¿½?¿½Ìˆï¿½
 */
void CDebugUtyAsm::OnPaint(HDC hDC, const RECT& rect)
{
	UINT nIndex = m_lpView->GetVScrollPos();

	if (m_address.size() < nIndex)
	{
		UINT nOffset = (!m_address.empty()) ? m_address.back() : m_nOffset;
		do
		{
			unsigned char sBuf[16];
			ReadMemory(nOffset, sBuf, sizeof(sBuf));

			UINT nStep = ::unasm(NULL, sBuf, sizeof(sBuf), FALSE, nOffset);
			if (nStep == 0)
			{
				return;
			}

			nOffset = (nOffset + nStep) & 0xffff;
			m_address.push_back(nOffset);
		} while (m_address.size() < nIndex);
	}

	UINT nOffset = (nIndex) ? m_address[nIndex - 1] : m_nOffset;
	for (int y = 0; (y < rect.bottom) && (nIndex < 0x1000); y += 16, nIndex++)
	{
		TCHAR szTmp[16];
		::wsprintf(szTmp, _T("%04x:%04x"), m_nSegment, nOffset);
		::TextOut(hDC, 0, y, szTmp, 9);

#if defined(SUPPORT_NP2BREAK)
		const UINT32 nAddress = (m_nSegment << 4) + nOffset;
		if (viewcmn_has_breakpoint(nAddress))
		{
			const TCHAR c = viewcmn_breakpoint_char(nAddress);
			::TextOut(hDC, 9 * 8, y, &c, 1);
		}
#endif

		unsigned char sBuf[16];
		ReadMemory(nOffset, sBuf, sizeof(sBuf));

		_UNASM una;
		UINT nStep = ::unasm(&una, sBuf, sizeof(sBuf), FALSE, nOffset);
		if (nStep == 0)
		{
			return;
		}

		::TextOutA(hDC, 11 * 8, y, una.mnemonic, ::lstrlenA(una.mnemonic));
		if (una.operand[0])
		{
			::TextOutA(hDC, (11 + 7) * 8, y, una.operand, ::lstrlenA(una.operand));
		}

		nOffset = (nOffset + nStep) & 0xffff;
		if (m_address.size() == nIndex)
		{
			m_address.push_back(nOffset);
		}
	}
}

/**
 * ??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½??¿½?¿½æ“¾
 * @param[in] nOffset ??¿½?¿½I??¿½?¿½t??¿½?¿½Z??¿½?¿½b??¿½?¿½g
 * @param[out] lpBuffer ??¿½?¿½o??¿½?¿½b??¿½?¿½t??¿½?¿½@
 * @param[in] cbBuffer ??¿½?¿½o??¿½?¿½b??¿½?¿½t??¿½?¿½@??¿½?¿½??¿½?¿½
 */
void CDebugUtyAsm::ReadMemory(UINT nOffset, unsigned char* lpBuffer, UINT cbBuffer)
{
	while (cbBuffer)
	{
		const UINT nLimit = min(nOffset + cbBuffer, 0x10000);
		const UINT nSize = nLimit - nOffset;

		if (!m_buffer.empty())
		{
			CopyMemory(lpBuffer, &m_buffer.at(nOffset), nSize);
		}
		else
		{
			m_mem.Read((m_nSegment << 4) + nOffset, lpBuffer, nSize);
		}

		nOffset = (nOffset + nSize) & 0xffff;
		lpBuffer += nSize;
		cbBuffer -= nSize;
	}
}

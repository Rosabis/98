/**
 * @file	viewasm.cpp
 * @brief	�A�Z���u�� ���X�g�\���N���X�̓���̒�`���s���܂�
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
 * �R���X�g���N�^
 * @param[in] lpView �r���[�� �C���X�^���X
 */
CDebugUtyAsm::CDebugUtyAsm(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODEASM)
	, m_nSegment(0)
	, m_nOffset(0)
{
}

/**
 * �f�X�g���N�^
 */
CDebugUtyAsm::~CDebugUtyAsm()
{
}

/**
 * ������
 * @param[in] lpItem ��ƂȂ�A�C�e��
 */
void CDebugUtyAsm::Initialize(const CDebugUtyItem* lpItem)
{
	m_nSegment = CPU_CS;
	m_nOffset = CPU_IP;
	m_lpView->SetVScroll(0, 0x1000);
}

/**
 * �X�V
 * @retval true �X�V����
 * @retval false �X�V�Ȃ�
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
 * ���b�N
 * @retval true ����
 * @retval false ���s
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
 * �A�����b�N
 */
void CDebugUtyAsm::Unlock()
{
	m_buffer.clear();
	m_address.clear();
}

/**
 * ���b�N��?
 * @retval true ���b�N���ł���
 * @retval false ���b�N���łȂ�
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
 * 物理アドレスへ移動する
 * @param[in] addr 物理アドレス
 */
void CDebugUtyAsm::SetAddress(UINT32 addr)
{
	m_nSegment = static_cast<UINT>((addr >> 4) & 0xf000);
	m_nOffset = static_cast<UINT>(addr & 0xffff);
	m_address.clear();
	m_lpView->SetVScrollPos(0);
}

/**
 * 物理アドレスから読み出す
 * @param[in] nAddress 物理アドレス
 * @param[in] lpBuffer バッファ
 * @param[in] cbBuffer バッファサイズ
 */
void CDebugUtyAsm::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
{
	m_mem.Read(nAddress, lpBuffer, cbBuffer);
}

/**
 * �`��
 * @param[in] hDC �f�o�C�X �R���e�L�X�g
 * @param[in] rect �̈�
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
 * �������擾
 * @param[in] nOffset �I�t�Z�b�g
 * @param[out] lpBuffer �o�b�t�@
 * @param[in] cbBuffer �o�b�t�@��
 */
void CDebugUtyAsm::ReadMemory(UINT nOffset, unsigned char* lpBuffer, UINT cbBuffer) const
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

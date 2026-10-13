/**
 * @file	viewreg.cpp
 * @brief	�������\���N���X�̓���̒�`���s���܂�
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewseg.h"
#include "viewer.h"
#include "viewcmn.h"
#include "cpucore.h"

/**
 * �R���X�g���N�^
 * @param[in] lpView �r���[�� �C���X�^���X
 */
CDebugUtySeg::CDebugUtySeg(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODESEG)
	, m_nSegment(0)
{
}

/**
 * �f�X�g���N�^
 */
CDebugUtySeg::~CDebugUtySeg()
{
}

/**
 * ������
 * @param[in] lpItem ��ƂȂ�A�C�e��
 */
void CDebugUtySeg::Initialize(const CDebugUtyItem* lpItem)
{
	m_lpView->SetVScroll(0, 0x1000);
}

/**
 * �X�V
 * @retval true �X�V����
 * @retval false �X�V�Ȃ�
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
 * ���b�N
 * @retval true ����
 * @retval false ���s
 */
bool CDebugUtySeg::Lock()
{
	m_buffer.resize(0x10000);

	m_mem.Update();
	m_mem.Read(m_nSegment << 4, &m_buffer.at(0), static_cast<UINT>(m_buffer.size()));
	return true;
}

/**
 * �A�����b�N
 */
void CDebugUtySeg::Unlock()
{
	m_buffer.clear();
}

/**
 * ���b�N��?
 * @retval true ���b�N���ł���
 * @retval false ���b�N���łȂ�
 */
bool CDebugUtySeg::IsLocked()
{
	return (!m_buffer.empty());
}

/**
 * 現在カーソルがある物理アドレスを返す
 * @return 物理アドレス
 */
UINT32 CDebugUtySeg::GetAddress() const
{
	return (m_nSegment + m_lpView->GetVScrollPos()) << 4;
}

/**
 * 現在のセグメントを返す
 * @return セグメント
 */
UINT CDebugUtySeg::GetSegment() const
{
	return m_nSegment;
}

/**
 * 物理アドレスへ移動する
 * @param[in] addr 物理アドレス
 */
void CDebugUtySeg::SetAddress(UINT32 addr)
{
	// スクロール位置はセグメント内の 16 バイト単位
	SetSegment(static_cast<UINT>((addr >> 4) & 0xf000));
	m_lpView->SetVScrollPos(static_cast<UINT>((addr >> 4) & 0x0fff));
}

/**
 * 物理アドレスから読み出す
 * @param[in] nAddress 物理アドレス
 * @param[in] lpBuffer バッファ
 * @param[in] cbBuffer バッファサイズ
 */
void CDebugUtySeg::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
{
	m_mem.Read(nAddress, lpBuffer, cbBuffer);
}

/**
 * ���[�U�[�����j���[�̍��ڂ�I�������Ƃ��ɁA�t���[�����[�N�ɂ���ČĂяo����܂�
 * @param[in] wParam �p�����^
 * @param[in] lParam �p�����^
 * @retval TRUE �A�v���P�[�V���������̃��b�Z�[�W����������
 * @retval FALSE �A�v���P�[�V���������̃��b�Z�[�W���������Ȃ�����
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
 * �Z�O�����g�ύX
 * @param[in] nSegment �Z�O�����g
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
 * �`��
 * @param[in] hDC �f�o�C�X �R���e�L�X�g
 * @param[in] rect �̈�
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

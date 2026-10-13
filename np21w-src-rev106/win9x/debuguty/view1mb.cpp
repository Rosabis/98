/**
 * @file	view1mb.cpp
 * @brief	���C�� �������\���N���X�̓���̒�`���s���܂�
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "view1mb.h"
#include "viewer.h"
#include "viewcmn.h"
#include "cpucore.h"

/**
 * �R���X�g���N�^
 * @param[in] lpView �r���[�� �C���X�^���X
 */
CDebugUty1MB::CDebugUty1MB(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODE1MB)
{
}

/**
 * �f�X�g���N�^
 */
CDebugUty1MB::~CDebugUty1MB()
{
}

/**
 * ������
 * @param[in] lpItem ��ƂȂ�A�C�e��
 */
void CDebugUty1MB::Initialize(const CDebugUtyItem* lpItem)
{
	m_lpView->SetVScroll(0, 0x10fff);
}

/**
 * �X�V
 * @retval true �X�V����
 * @retval false �X�V�Ȃ�
 */
bool CDebugUty1MB::Update()
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
bool CDebugUty1MB::Lock()
{
	m_buffer.resize(0x10fff0);

	m_mem.Update();
	m_mem.Read(0, &m_buffer.at(0), static_cast<UINT>(m_buffer.size()));
	return true;
}

/**
 * �A�����b�N
 */
void CDebugUty1MB::Unlock()
{
	m_buffer.clear();
}

/**
 * ���b�N��?
 * @retval true ���b�N���ł���
 * @retval false ���b�N���łȂ�
 */
bool CDebugUty1MB::IsLocked()
{
	return (!m_buffer.empty());
}

/**
 * 現在カーソルがある物理アドレスを返す
 * @return 物理アドレス
 */
UINT32 CDebugUty1MB::GetAddress() const
{
	return m_lpView->GetVScrollPos() << 4;
}

/**
 * 現在のセグメントを返す
 * @return セグメント
 */
UINT CDebugUty1MB::GetSegment() const
{
	return 0;
}

/**
 * 物理アドレスへ移動する
 * @param[in] addr 物理アドレス
 */
void CDebugUty1MB::SetAddress(UINT32 addr)
{
	m_lpView->SetVScrollPos(static_cast<UINT>((addr >> 4) & 0xffff));
}

/**
 * 物理アドレスから読み出す
 * @param[in] nAddress 物理アドレス
 * @param[in] lpBuffer バッファ
 * @param[in] cbBuffer バッファサイズ
 */
void CDebugUty1MB::ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer)
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
BOOL CDebugUty1MB::OnCommand(WPARAM wParam, LPARAM lParam)
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
void CDebugUty1MB::SetSegment(UINT nSegment)
{
	m_lpView->SetVScrollPos(nSegment);
}

/**
 * �`��
 * @param[in] hDC �f�o�C�X �R���e�L�X�g
 * @param[in] rect �̈�
 */
void CDebugUty1MB::OnPaint(HDC hDC, const RECT& rect)
{
	UINT nIndex = m_lpView->GetVScrollPos();
	for (int y = 0; (y < rect.bottom) && (nIndex < 0x10fff); y += 16, nIndex++)
	{
		TCHAR szTmp[16];
		::wsprintf(szTmp, _T("%08x:"), nIndex << 4);
		::TextOut(hDC, 0, y, szTmp, 9);

#if defined(SUPPORT_NP2BREAK)
		const UINT32 nAddress = nIndex << 4;
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
			m_mem.Read(nIndex << 4, sBuf, sizeof(sBuf));
		}
		for (int x = 0; x < 16; x++)
		{
			TCHAR szTmp[4];
			::wsprintf(szTmp, TEXT("%02X"), sBuf[x]);
			::TextOut(hDC, ((x * 3) + 11) * 8, y, szTmp, 2);
		}
	}
}

/**
 * @file	viewstk.cpp
 * @brief	スタック ビュークラスの実装 (np2debug より移植)
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewstk.h"
#include "viewer.h"
#include "viewcmn.h"
#include "cpucore.h"

/**
 * コンストラクタ
 * @param[in] lpView ビュー
 */
CDebugUtyStk::CDebugUtyStk(CDebugUtyView* lpView)
	: CDebugUtyItem(lpView, IDM_VIEWMODESTK)
	, m_nSegment(0)
	, m_nOffset(0)
{
}

/**
 * デストラクタ
 */
CDebugUtyStk::~CDebugUtyStk()
{
}

/**
 * 初期化
 * @param[in] lpItem 引き継ぐアイテム
 */
void CDebugUtyStk::Initialize(const CDebugUtyItem* lpItem)
{
	SetStack();
}

/**
 * 更新
 * @retval true 更新あり
 * @retval false 更新なし
 */
bool CDebugUtyStk::Update()
{
	if (!m_buffer.empty())
	{
		return false;
	}
	SetStack();
	return true;
}

/**
 * ロック
 * @retval true 成功
 * @retval false 失敗
 */
bool CDebugUtyStk::Lock()
{
	m_buffer.resize(0x10000);

	SetStack();
	m_mem.Update();
	m_mem.Read(m_nSegment << 4, &m_buffer.at(0), static_cast<UINT>(m_buffer.size()));
	return true;
}

/**
 * アンロック
 */
void CDebugUtyStk::Unlock()
{
	m_buffer.clear();
}

/**
 * ロック中?
 * @retval true ロック中
 * @retval false ロックされていない
 */
bool CDebugUtyStk::IsLocked()
{
	return (!m_buffer.empty());
}

/**
 * 現在カーソルがある物理アドレスを返す
 * @return 物理アドレス
 */
UINT32 CDebugUtyStk::GetAddress() const
{
	return ((m_nSegment + m_lpView->GetVScrollPos()) << 4) + 0;
}

/**
 * 現在のセグメントを返す
 * @return セグメント
 */
UINT CDebugUtyStk::GetSegment() const
{
	return m_nSegment;
}

/**
 * WM_COMMAND メッセージの処理
 * @param[in] wParam パラメータ
 * @param[in] lParam パラメータ
 * @retval TRUE 処理した
 * @retval FALSE 処理していない
 */
BOOL CDebugUtyStk::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case IDM_SEGSS:
			SetStack();
			m_lpView->Invalidate();
			break;

		default:
			return FALSE;
	}
	return TRUE;
}

/**
 * スタック位置を設定する
 */
void CDebugUtyStk::SetStack()
{
	m_nSegment = CPU_SS;
	m_nOffset = CPU_SP;
	m_lpView->SetVScroll(0, 0x1000);
	m_lpView->SetVScrollPos(m_nOffset >> 4);
}

/**
 * 描画
 * @param[in] hDC デバイス コンテキスト
 * @param[in] rect 領域
 */
void CDebugUtyStk::OnPaint(HDC hDC, const RECT& rect)
{
	UINT nIndex = m_lpView->GetVScrollPos();
	for (int y = 0; (y < rect.bottom) && (nIndex < 0x1000); y += 16, nIndex++)
	{
		const UINT nOffset = nIndex << 4;

		TCHAR szTmp[16];
		::wsprintf(szTmp, _T("%04x:%04x"), m_nSegment, nOffset);
		::TextOut(hDC, 0, y, szTmp, 9);

#if defined(SUPPORT_NP2BREAK)
		if (viewcmn_has_breakpoint((m_nSegment << 4) + nOffset))
		{
			const TCHAR c = viewcmn_breakpoint_char((m_nSegment << 4) + nOffset);
			::TextOut(hDC, 9 * 8, y, &c, 1);
		}
#endif

		unsigned char sBuf[16];
		if (!m_buffer.empty())
		{
			CopyMemory(sBuf, &m_buffer.at(nOffset), sizeof(sBuf));
		}
		else
		{
			m_mem.Read((m_nSegment << 4) + nOffset, sBuf, sizeof(sBuf));
		}
		for (int x = 0; x < 16; x++)
		{
			TCHAR szTmp[4];
			::wsprintf(szTmp, TEXT("%02X"), sBuf[x]);
			::TextOut(hDC, ((x * 3) + 11) * 8, y, szTmp, 2);
		}
	}
}

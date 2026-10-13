/**
 * @file	viewitem.h
 * @brief	DebugUty �p�r���[���N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂�
 */

#pragma once

class CDebugUtyView;

/**
 * @brief �f�o�O�\���A�C�e���̊��N���X
 */
class CDebugUtyItem
{
public:
	static CDebugUtyItem* New(UINT nID, CDebugUtyView* lpView, const CDebugUtyItem* lpItem = NULL);

	CDebugUtyItem(CDebugUtyView* lpView, UINT nID);
	virtual ~CDebugUtyItem();
	UINT GetID() const;

	//! 現在カーソルがある物理アドレス (ステータスバー / ブレークポイント用)
	virtual UINT32 GetAddress() const;
	//! 現在のセグメント
	virtual UINT GetSegment() const;

	//! 物理アドレスへ移動する (d_addr ダイアログ / 検索結果)
	virtual void SetAddress(UINT32 addr);

	//! 物理アドレスから読み出す (ビューごとのメモリ設定を反映)
	virtual void ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer);

	//! 物理アドレスへ書き込む (d_edit ダイアログ)
	virtual void WriteMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes);

	//! 物理アドレス以降からバイト列を検索する
	virtual BOOL FindMemory(UINT32 nAddress, const UINT8* lpBytes, UINT cbBytes, UINT32* lpFound);

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC, const RECT& rect);

protected:
	CDebugUtyView* m_lpView;	//!< �r���[ �N���X
	UINT m_nID;					//!< ID
};

/**
 * ID ��Ԃ�
 * @return ID
 */
inline UINT CDebugUtyItem::GetID() const
{
	return m_nID;
}

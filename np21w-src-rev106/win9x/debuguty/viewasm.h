/**
 * @file	viewasm.h
 * @brief	�A�Z���u�� ���X�g�\���N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂�
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief �A�Z���u�� ���X�g�\���N���X
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
	UINT m_nSegment;							//!< �Z�O�����g
	UINT m_nOffset;								//!< �Z�O�����g
	DebugUtyViewMemory m_mem;					//!< ������
	std::vector<unsigned char> m_buffer;		//!< �o�b�t�@
	std::vector<UINT> m_address;				//!< �A�h���X �o�b�t�@
	void ReadMemory(UINT nOffset, unsigned char* lpBuffer, UINT cbBuffer) const;
};

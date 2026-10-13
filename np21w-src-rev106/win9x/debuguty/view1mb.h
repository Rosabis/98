/**
 * @file	view1mb.h
 * @brief	���C�� ������ ���W�X�^�\���N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂�
 */

#pragma once

#include <vector>
#include "viewitem.h"
#include "viewmem.h"

/**
 * @brief ������ ���W�X�^�\���N���X
 */
class CDebugUty1MB : public CDebugUtyItem
{
public:
	CDebugUty1MB(CDebugUtyView* lpView);
	virtual ~CDebugUty1MB();

	virtual void Initialize(const CDebugUtyItem* lpItem = NULL);
	virtual bool Update();
	virtual bool Lock();
	virtual void Unlock();
	virtual bool IsLocked();
	virtual UINT32 GetAddress() const;
	virtual UINT GetSegment() const;
	virtual void SetAddress(UINT32 addr);
	virtual void ReadMemory(UINT32 nAddress, LPVOID lpBuffer, UINT32 cbBuffer);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC, const RECT& rect);

private:
	DebugUtyViewMemory m_mem;					//!< ������
	std::vector<unsigned char> m_buffer;		//!< �o�b�t�@
	void SetSegment(UINT nSegment);
};

/**
 * @file	d_disk.cpp
 * @brief	disk dialog
 */

#include "compiler.h"
#include "resource.h"
#include "dialog.h"
#include "c_combodata.h"
#include "dosio.h"
#include "np2.h"
#include "np2mt.h"
#include "sysmng.h"
#include "misc/DlgProc.h"
#include "subwnd/toolwnd.h"
#include "pccore.h"
#include "common/strres.h"
#include "fdd/diskdrv.h"
#include "diskimage/fddfile.h"
#include "fdd/newdisk.h"
#include "fdd/sxsi.h"
#include "np2class.h"
#include <shlobj.h>
#if defined(_WINDOWS)
#include	<process.h>
#endif
#ifdef SUPPORT_NVL_IMAGES
extern "C" BOOL nvl_check();
#endif


// �i���\���p�i��������������j
static int mt_progressvalue = 0;
static int mt_progressmax = 100;

/**
 * FDD �I���_�C�A���O
 * @param[in] hWnd �e�E�B���h�E
 * @param[in] drv �h���C�u
 */
void dialog_changefdd(HWND hWnd, REG8 drv)
{
	if (drv < 4)
	{
		LPCTSTR lpPath = fdd_diskname(drv);
		if ((lpPath == NULL) || (lpPath[0] == '\0'))
		{
			lpPath = fddfolder;
		}

		std::tstring rExt(LoadTString(IDS_FDDEXT));
		std::tstring rFilter(LoadTString(IDS_FDDFILTER));
		std::tstring rTitle(LoadTString(IDS_FDDTITLE));

		CFileDlg dlg(TRUE, rExt.c_str(), lpPath, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, rFilter.c_str(), hWnd);
		dlg.m_ofn.lpstrTitle = rTitle.c_str();
		dlg.m_ofn.nFilterIndex = 8; // 3;
		if (dlg.DoModal())
		{
			LPCTSTR lpImage = dlg.GetPathName();
			BOOL bReadOnly = dlg.GetReadOnlyPref();

			file_cpyname(fddfolder, lpImage, _countof(fddfolder));
			sysmng_update(SYS_UPDATEOSCFG);
			np2_multithread_Suspend();
			diskdrv_setfdd(drv, lpImage, bReadOnly);
			toolwin_setfdd(drv, lpImage);
			np2_multithread_Resume();
		}
	}
}

/**
 * HDD �I���_�C�A���O
 * @param[in] hWnd �e�E�B���h�E
 * @param[in] drv �h���C�u
 */
void dialog_changehdd(HWND hWnd, REG8 drv)
{
	const UINT num = drv & 0x0f;

	UINT nTitle = 0;
	UINT nExt = 0;
	UINT nFilter = 0;
	UINT nIndex = 0;

	if (!(drv & 0x20))			// SASI/IDE
	{
#if defined(SUPPORT_IDEIO)
		if (num < 4)
		{
			if(sxsi_getdevtype(drv)!=SXSIDEV_CDROM)
			{
				nTitle = IDS_SASITITLE;
				nExt = IDS_HDDEXT;
				nFilter = IDS_HDDFILTER;
				//nIndex = 6;
				nIndex = 0;
			}
			else
			{
				nTitle = IDS_ISOTITLE;
				nExt = IDS_ISOEXT;
				nFilter = IDS_ISOFILTER;
				//nIndex = 7; // 3
				nIndex = 0;
			}
		}
#else
		if (num < 2)
		{
#if defined(SUPPORT_SASI)
			nTitle = IDS_SASITITLE;
#else
			nTitle = IDS_HDDTITLE;
#endif
			nExt = IDS_HDDEXT;
			nFilter = IDS_HDDFILTER;
			//nIndex = 6;//4;
			nIndex = 0;
		}
#endif
	}
#if defined(SUPPORT_SCSI)
	else						// SCSI
	{
		if (num < 4)
		{
			nTitle = IDS_SCSITITLE;
			nExt = IDS_SCSIEXT;
			nFilter = IDS_SCSIFILTER;
			//nIndex = 3;	
			nIndex = 0;
		}
	}
#endif	// defined(SUPPORT_SCSI)
	if (nExt == 0)
	{
		return;
	}

	LPCTSTR lpPath;
#ifdef SUPPORT_IDEIO
	if(np2cfg.idetype[drv]!=SXSIDEV_CDROM)
	{
#endif
		lpPath = diskdrv_getsxsi(drv);
#ifdef SUPPORT_IDEIO
	}
	else
	{
		lpPath = np2cfg.idecd[drv];
	}
#endif
	if ((lpPath == NULL) || (lpPath[0] == '\0') || _tcsnicmp(lpPath, OEMTEXT("\\\\.\\"), 4)==0)
	{
		lpPath = sxsi_getfilename(drv);
		if ((lpPath == NULL) || (lpPath[0] == '\0') || _tcsnicmp(lpPath, OEMTEXT("\\\\.\\"), 4)==0)
		{
			if(sxsi_getdevtype(drv)!=SXSIDEV_CDROM)
			{
				lpPath = hddfolder;
			}
			else
			{
				lpPath = cdfolder;
			}
		}
	}
	
#ifdef SUPPORT_NVL_IMAGES
	if(nFilter == IDS_HDDFILTER && nvl_check()){
		nFilter = IDS_HDDFILTER_NVL;
		nIndex = 0;
	}
#endif

	std::tstring rExt(LoadTString(nExt));
	std::tstring rFilter(LoadTString(nFilter));
	std::tstring rTitle(LoadTString(nTitle));
	
	if(nIndex==0){ // All supported files�i��납��2�Ԗځj�������I��
		int seppos = 0;
		int seppostmp;
		int sepcount = 0;
		// ��؂蕶���̐��𐔂���
		while((seppostmp = (int)rFilter.find('|', seppos)) != std::string::npos){
			if(seppostmp == std::string::npos) break;
			seppos = seppostmp + 1;
			sepcount++;
		}
		if(rFilter.back()!='|'){
			sepcount++; // ������|�łȂ���΂�����̂Ƃ���
		}
		if((sepcount / 2) - 1 > 0){
			nIndex = (sepcount / 2) - 1; // �ŌオAll files�Ȃ̂ň�O��I��
		}
	}

	CFileDlg dlg(TRUE, rExt.c_str(), lpPath, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_SHAREAWARE, rFilter.c_str(), hWnd);
	dlg.m_ofn.lpstrTitle = rTitle.c_str();
	dlg.m_ofn.nFilterIndex = nIndex;
	if (dlg.DoModal())
	{
		LPCTSTR lpImage = dlg.GetPathName();
#ifdef SUPPORT_IDEIO
		if(np2cfg.idetype[drv]!=SXSIDEV_CDROM)
		{
			file_cpyname(hddfolder, lpImage, _countof(hddfolder));
		}
		else
		{
#endif
			file_cpyname(cdfolder, lpImage, _countof(cdfolder));
#ifdef SUPPORT_IDEIO
		}
#endif
		sysmng_update(SYS_UPDATEOSCFG);
		diskdrv_setsxsi(drv, lpImage);
	}
}

#if defined(SUPPORT_MOUNTDIR)
static int CALLBACK opendirproc(HWND hWnd, UINT msg, LPARAM lp, LPARAM data)
{
	switch (msg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, data);
			break;

		case BFFM_VALIDATEFAILED:
			return 1;
	}
	return 0;
}

/**
 * mountdir: select a host directory and mount it to a DOS drive
 * @param[in] hWnd parent window
 * @param[in] drv DOS drive index (0 = A:)
 */
void dialog_changedir(HWND hWnd, REG8 drv)
{
	OEMCHAR title[256];
	OEMCHAR drive[2];
	BROWSEINFO bi;
	OEMCHAR display_name[MAX_PATH];
	PIDLIST_ABSOLUTE ret;

	if (drv >= MAX_DIR)
	{
		return;
	}

	drive[0] = (OEMCHAR)('A' + drv);
	drive[1] = '\0';
	// "Select directory to mount to DOS drive X:\:"
	wsprintf(title, _T("Select directory to mount to DOS drive %s:\\:"), drive);

	ZeroMemory(&bi, sizeof(bi));
	bi.hwndOwner = hWnd;
	bi.pszDisplayName = display_name;
	bi.lpszTitle = title;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON | BIF_USENEWUI;
	bi.lpfn = opendirproc;
	bi.lParam = (LPARAM)np2cfg.mountdir[drv];

	ret = SHBrowseForFolder(&bi);
	if (ret)
	{
		SHGetPathFromIDList(ret, np2cfg.mountdir[drv]);
		CoTaskMemFree(ret);
		sysmng_update(SYS_UPDATEOSCFG);
	}
}
#endif


// ---- newdisk

/** �f�t�H���g�� */
static const TCHAR str_newdisk[] = TEXT("newdisk");

/** HDD �T�C�Y */
#ifdef SUPPORT_LARGE_HDD
static const UINT32 s_hddsizetbl[] = {20, 41, 65, 80, 127, 255, 511, 1023, 2047, 4095, 8191};
#else
static const UINT32 s_hddsizetbl[] = {20, 41, 65, 80, 127, 255, 511, 1023, 2047};
#endif

/** HDD �T�C�Y */
static const UINT32 s_hddCtbl[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65535};
static const UINT32 s_hddHtbl[] = { 8, 15, 16};
static const UINT32 s_hddStbl[] = {17, 63, 255};
static const UINT32 s_hddSStbl[] = {256, 512};
static const UINT32 s_hddHtblSCSI[] = { 8, 15 };

/** SASI HDD */
static const UINT16 s_sasires[6] = 
{
	IDC_NEWSASI5MB, IDC_NEWSASI10MB,
	IDC_NEWSASI15MB, IDC_NEWSASI20MB,
	IDC_NEWSASI30MB, IDC_NEWSASI40MB
};

/** SASI HDD CHS (SS=256) */
static const SASIHDD s_sasihddtbl[] = {
	{33, 4, 153},			// 5MB
	{33, 4, 310},			// 10MB
	{33, 6, 310},			// 15MB
	{33, 8, 310},			// 20MB
	{33, 6, 615},			// 30MB
	{33, 8, 615}};			// 40MB

/**
 * @brief �V����HDD
 */
class CNewHddDlg : public CDlgProc
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] hwndParent �e�E�B���h�E
	 * @param[in] nHddMinSize �ŏ��T�C�Y
	 * @param[in] nHddMaxSize �ő�T�C�Y
	 */
	CNewHddDlg(HWND hwndParent, UINT32 nHddMinSize, UINT32 nHddMaxSize, UINT8 allowsasi, UINT8 isSCSI = 0)
		: CDlgProc(IDD_NEWHDDDISK, hwndParent)
		, m_nHddSize(0)
		, m_nHddMinSize(nHddMinSize)
		, m_nHddMaxSize(nHddMaxSize)
		, m_advanced(0)
		, m_isSCSI(isSCSI)
		, m_usedynsize(0)
		, m_HddC(0)
		, m_HddH(0)
		, m_HddS(0)
		, m_HddSS(0)
		, m_dynsize(0)
		, m_blank(0)
		, m_allowsasi(allowsasi)
	{
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CNewHddDlg()
	{
	}

	/**
	 * �T�C�Y��Ԃ�
	 * @return �T�C�Y
	 */
	UINT32 GetSize() const
	{
		return m_nHddSize;
	}
	
	/**
	 * �V�����_����Ԃ�
	 * @return �T�C�Y
	 */
	UINT GetC() const
	{
		return m_HddC;
	}
	
	/**
	 * �w�b�h����Ԃ�
	 * @return �T�C�Y
	 */
	UINT GetH() const
	{
		return m_HddH;
	}
	
	/**
	 * �Z�N�^����Ԃ�
	 * @return �T�C�Y
	 */
	UINT GetS() const
	{
		return m_HddS;
	}
	
	/**
	 * �Z�N�^�T�C�Y��Ԃ�
	 * @return �T�C�Y
	 */
	UINT GetSS() const
	{
		return m_HddSS;
	}
	
	/**
	 * �ڍאݒ胂�[�h�Ȃ�true
	 * @return �ڍאݒ胂�[�h
	 */
	bool IsAdvancedMode() const
	{
		return (m_advanced != 0);
	}
	
	/**
	 * �e�ʉσ��[�h�Ȃ�true
	 * @return �ڍאݒ胂�[�h
	 */
	bool IsDynamicDisk() const
	{
		return (m_dynsize != 0);
	}
	
	/**
	 * ��f�B�X�N�쐬�Ȃ�true
	 * @return �ڍאݒ胂�[�h
	 */
	bool IsBlankDisk() const
	{
		return (m_blank != 0);
	}
	
	/**
	 * �g���ݒ������
	 * @return 
	 */
	void EnableAdvancedOptions()
	{
		m_advanced = 1;
		GetDlgItem(IDC_HDDADVANCED).EnableWindow(TRUE);
	}
	
	/**
	 * ���I���T�C�Y�f�B�X�N������(VHD�p)
	 * @return 
	 */
	void EnableDynamicSize()
	{
		m_usedynsize = 1;
		GetDlgItem(IDC_HDDADVANCED_FIXSIZE).EnableWindow(TRUE);
		GetDlgItem(IDC_HDDADVANCED_DYNSIZE).EnableWindow(TRUE);
	}
	
	/**
	 * �f�B�X�N�T�C�Y����CHS���������肵�ĕ\������
	 * @return 
	 */
	void SetCHSfromSize()
	{
		//UINT16 C,H,S,SS;
		//UINT32 hddsize; // disk size(MB
		
		//hddsize = GetDlgItemInt(IDC_HDDSIZE, NULL, FALSE);

		if (m_isSCSI)
		{
			if (m_nHddSize < 8192)
			{
				m_HddH = 8;
				m_HddS = 32;
				m_HddSS = 512;
			}
			else if (m_nHddSize < 32767)
			{
				m_HddH = 8;
				m_HddS = 128;
				m_HddSS = 512;
			}
			else
			{
				m_HddH = 15;
				m_HddS = 255;
				m_HddSS = 512;
			}
		}
		else
		{
			if (m_nHddSize < 4351)
			{
				m_HddH = 8;
				m_HddS = 17;
				m_HddSS = 512;
			}
			else if (m_nHddSize < 32255)
			{
				m_HddH = 16;
				m_HddS = 63;
				m_HddSS = 512;
			}
			else
			{
				m_HddH = 16;
				m_HddS = 255;
				m_HddSS = 512;
			}
		}
		m_HddC = (UINT32)((FILELEN)m_nHddSize * 1024 * 1024 / m_HddH / m_HddS / m_HddSS);

		SetDlgItemInt(IDC_HDDADVANCED_C, m_HddC, FALSE);
		SetDlgItemInt(IDC_HDDADVANCED_H, m_HddH, FALSE);
		SetDlgItemInt(IDC_HDDADVANCED_S, m_HddS, FALSE);
		SetDlgItemInt(IDC_HDDADVANCED_SS, m_HddSS, FALSE);
	}
	
	/**
	 * CHS����f�B�X�N�T�C�Y�ɕϊ����ĕ\������
	 * @return 
	 */
	void SetSizefromCHS()
	{
		m_nHddSize = (UINT32)((FILELEN)m_HddC * m_HddH * m_HddS * m_HddSS / 1024 / 1024);

		int sasiidx = NELEMENTS(s_sasihddtbl);
		if(m_allowsasi && m_HddSS==256){
			for(sasiidx=0;sasiidx<NELEMENTS(s_sasihddtbl);sasiidx++){
				if(m_HddC==s_sasihddtbl[sasiidx].cylinders && m_HddH==s_sasihddtbl[sasiidx].surfaces && m_HddS==s_sasihddtbl[sasiidx].sectors){
					break;
				}
			}
		}
		if(sasiidx < NELEMENTS(s_sasihddtbl)){
			m_hddsize.SetCurSel(sasiidx);
		}else{
			SetDlgItemInt(IDC_HDDSIZE, m_nHddSize, FALSE);
		}
	}
	
	/**
	 * �A�C�e���̗̈�𓾂�
	 * @param[in] nID ID
	 * @param[out] rect �̈�
	 */
	void GetDlgItemRect(UINT nID, RECT& rect)
	{
		CWndBase wnd = GetDlgItem(nID);
		wnd.GetWindowRect(&rect);
		::MapWindowPoints(HWND_DESKTOP, m_hWnd, reinterpret_cast<LPPOINT>(&rect), 2);
	}

protected:
	/**
	 * ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂�
	 * @retval TRUE �ŏ��̃R���g���[���ɓ��̓t�H�[�J�X��ݒ�
	 * @retval FALSE ���ɐݒ��
	 */
	virtual BOOL OnInitDialog()
	{
		int hddsizetblcount = 0;
		
		while(hddsizetblcount<_countof(s_hddsizetbl) && s_hddsizetbl[hddsizetblcount] <= m_nHddMaxSize){
			hddsizetblcount++;
		}

		m_hddsize.SubclassDlgItem(IDC_HDDSIZE, this);
		if(m_allowsasi){
			for(int i=0;i<NELEMENTS(s_sasihddtbl);i++){
				TCHAR sasistr[32];
				OEMSPRINTF(sasistr, TEXT("(SASI) %d"), 
					(s_sasihddtbl[i].cylinders * s_sasihddtbl[i].surfaces * s_sasihddtbl[i].sectors * 256 / 1024 / 1024 + 2) / 5 * 5); // ����5MB�P�ʕ\��
				m_hddsize.Add(sasistr, 0xffffffff-i); // ���蓾�Ȃ��l�ɂ��Ă���
			}
		}
		m_hddsize.Add(s_hddsizetbl, hddsizetblcount);
		
		m_cmbhddC.SubclassDlgItem(IDC_HDDADVANCED_C, this);
		m_cmbhddC.Add(s_hddCtbl, _countof(s_hddCtbl));

		if (m_isSCSI)
		{
			m_cmbhddH.SubclassDlgItem(IDC_HDDADVANCED_H, this);
			m_cmbhddH.Add(s_hddHtblSCSI, _countof(s_hddHtblSCSI));
		}
		else
		{
			m_cmbhddH.SubclassDlgItem(IDC_HDDADVANCED_H, this);
			m_cmbhddH.Add(s_hddHtbl, _countof(s_hddHtbl));
		}

		m_cmbhddS.SubclassDlgItem(IDC_HDDADVANCED_S, this);
		m_cmbhddS.Add(s_hddStbl, _countof(s_hddStbl));

		m_cmbhddSS.SubclassDlgItem(IDC_HDDADVANCED_SS, this);
		m_cmbhddSS.Add(s_hddSStbl, _countof(s_hddSStbl));
		if(!m_allowsasi){
			m_HddSS = 512;
			SetDlgItemInt(IDC_HDDADVANCED_SS, m_HddSS, FALSE);
			m_cmbhddSS.EnableWindow(FALSE);
		}

		TCHAR work[32];
		::wsprintf(work, TEXT("(%u-%uMB)"), m_nHddMinSize, m_nHddMaxSize > NHD_MAXSIZE28 ? NHD_MAXSIZE28 : m_nHddMaxSize); // �\����28bit LBA����
		SetDlgItemText(IDC_HDDLIMIT, work);
		
		m_rdbfixsize.SubclassDlgItem(IDC_HDDADVANCED_FIXSIZE, this);
		m_rdbdynsize.SubclassDlgItem(IDC_HDDADVANCED_DYNSIZE, this);
		m_rdbfixsize.SendMessage(BM_SETCHECK , BST_CHECKED , 0);
		
		m_chkblank.SubclassDlgItem(IDC_HDDADVANCED_BLANK, this);
		m_chkblank.SendMessage(BM_SETCHECK , BST_UNCHECKED , 0);

		RECT rect;
		GetWindowRect(&rect);
		m_szNewDisk.cx = rect.right - rect.left;
		m_szNewDisk.cy = rect.bottom - rect.top;

		RECT rectMore;
		GetDlgItemRect(IDC_HDDADVANCED, rectMore);
		RECT rectInfo;
		GetDlgItemRect(IDC_HDDADVANCED_SS, rectInfo);
		const int nHeight = m_szNewDisk.cy - (rectInfo.bottom - rectMore.bottom);

		CWndBase wndParent = GetParent();
		wndParent.GetClientRect(&rect);

		POINT pt;
		pt.x = (rect.right - rect.left - m_szNewDisk.cx) / 2;
		pt.y = (rect.bottom - rect.top - m_szNewDisk.cy) / 2;
		wndParent.ClientToScreen(&pt);
		np2class_move(m_hWnd, pt.x, pt.y, m_szNewDisk.cx, nHeight);

		if(m_advanced) EnableAdvancedOptions();
		if(m_usedynsize) EnableDynamicSize();
		
		m_hddsize.SetFocus();
		
		return FALSE;
	}

	/**
	 * ���[�U�[�� OK �̃{�^�� (IDOK ID ���̃{�^��) ���N���b�N����ƌĂяo����܂�
	 */
	virtual void OnOK()
	{
		UINT nSize = m_nHddSize;
		nSize = max(nSize, m_nHddMinSize);
		nSize = min(nSize, m_nHddMaxSize);
		m_nHddSize = nSize;
		CDlgProc::OnOK();
	}
	
	/**
	 * ���[�U�[�����j���[�̍��ڂ�I�������Ƃ��ɁA�t���[�����[�N�ɂ���ČĂяo����܂�
	 * @param[in] wParam �p�����^
	 * @param[in] lParam �p�����^
	 * @retval TRUE �A�v���P�[�V���������̃��b�Z�[�W����������
	 */
	BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		switch (LOWORD(wParam))
		{
			case IDC_HDDADVANCED:
				RECT rect;
				GetWindowRect(&rect);
				np2class_move(m_hWnd, rect.left, rect.top, m_szNewDisk.cx, m_szNewDisk.cy);
				GetDlgItem(IDC_HDDADVANCED).EnableWindow(FALSE);
				GetDlgItem(IDC_HDDADVANCED_C).EnableWindow(TRUE);
				GetDlgItem(IDC_HDDADVANCED_H).EnableWindow(TRUE);
				GetDlgItem(IDC_HDDADVANCED_S).EnableWindow(TRUE);
				if(m_allowsasi){
					GetDlgItem(IDC_HDDADVANCED_SS).EnableWindow(TRUE);
				}
				GetDlgItem(IDC_HDDADVANCED_C).SetFocus();
				if(m_dynsize){
					GetDlgItem(IDC_HDDADVANCED_FIXSIZE).EnableWindow(TRUE);
					GetDlgItem(IDC_HDDADVANCED_DYNSIZE).EnableWindow(TRUE);
				}
				m_advanced = 1;
				return TRUE;
				
			case IDC_HDDSIZE:
				if (HIWORD(wParam) == CBN_EDITCHANGE){
					m_nHddSize = GetDlgItemInt(IDC_HDDSIZE, NULL, FALSE);
					SetCHSfromSize();
					return TRUE;
				}else if(HIWORD(wParam) == CBN_SELCHANGE) {
					int selindex = m_hddsize.GetCurSel();
					if(selindex!=CB_ERR){
						if(m_allowsasi){
							if(selindex < NELEMENTS(s_sasihddtbl)){
								m_HddC = s_sasihddtbl[selindex].cylinders;
								m_HddH = s_sasihddtbl[selindex].surfaces;
								m_HddS = s_sasihddtbl[selindex].sectors;
								m_HddSS = 256;
								m_nHddSize = (UINT32)((FILELEN)m_HddC * m_HddH * m_HddS * m_HddSS / 1024 / 1024);
								m_nHddSize = (m_nHddSize + 4) / 5 * 5; // 5MB�P�ʂɂ���
								SetDlgItemInt(IDC_HDDADVANCED_C, m_HddC, FALSE);
								SetDlgItemInt(IDC_HDDADVANCED_H, m_HddH, FALSE);
								SetDlgItemInt(IDC_HDDADVANCED_S, m_HddS, FALSE);
								SetDlgItemInt(IDC_HDDADVANCED_SS, m_HddSS, FALSE);
							}else{
								selindex -= NELEMENTS(s_sasihddtbl);
								m_nHddSize = s_hddsizetbl[selindex];
								SetCHSfromSize();
							}
						}else{
							m_nHddSize = s_hddsizetbl[selindex];
							SetCHSfromSize();
						}
					}
					return TRUE;
				}
				break;
				
			case IDC_HDDADVANCED_C:
				if (HIWORD(wParam) == CBN_EDITCHANGE){
					m_HddC = GetDlgItemInt(IDC_HDDADVANCED_C, NULL, FALSE);
					SetSizefromCHS();
					return TRUE;
				}else if(HIWORD(wParam) == CBN_SELCHANGE) {
					int selindex = m_cmbhddC.GetCurSel();
					if(selindex!=CB_ERR){
						m_HddC = s_hddCtbl[m_cmbhddC.GetCurSel()];
						SetSizefromCHS();
					}
					return TRUE;
				}
				break;
				
			case IDC_HDDADVANCED_H:
				if (HIWORD(wParam) == CBN_EDITCHANGE){
					m_HddH = GetDlgItemInt(IDC_HDDADVANCED_H, NULL, FALSE);
					SetSizefromCHS();
					return TRUE;
				}else if(HIWORD(wParam) == CBN_SELCHANGE) {
					int selindex = m_cmbhddH.GetCurSel();
					if(selindex!=CB_ERR){
						m_HddH = s_hddHtbl[m_cmbhddH.GetCurSel()];
						SetSizefromCHS();
					}
					return TRUE;
				}
				break;
				
			case IDC_HDDADVANCED_S:
				if (HIWORD(wParam) == CBN_EDITCHANGE){
					m_HddS = GetDlgItemInt(IDC_HDDADVANCED_S, NULL, FALSE);
					SetSizefromCHS();
					return TRUE;
				}else if(HIWORD(wParam) == CBN_SELCHANGE) {
					int selindex = m_cmbhddS.GetCurSel();
					if(selindex!=CB_ERR){
						m_HddS = s_hddStbl[m_cmbhddS.GetCurSel()];
						SetSizefromCHS();
					}
					return TRUE;
				}
				break;
				
			case IDC_HDDADVANCED_SS:
				if (HIWORD(wParam) == CBN_EDITCHANGE){
					m_HddSS = GetDlgItemInt(IDC_HDDADVANCED_SS, NULL, FALSE);
					SetSizefromCHS();
					return TRUE;
				}else if(HIWORD(wParam) == CBN_SELCHANGE) {
					int selindex = m_cmbhddSS.GetCurSel();
					if(selindex!=CB_ERR){
						m_HddSS = s_hddSStbl[m_cmbhddSS.GetCurSel()];
						SetSizefromCHS();
					}
					return TRUE;
				}
				break;
				
			case IDC_HDDADVANCED_FIXSIZE:
			case IDC_HDDADVANCED_DYNSIZE:
				m_dynsize = (m_rdbdynsize.SendMessage(BM_GETCHECK , 0 , 0) ? 1 : 0);
				return TRUE;
				
			case IDC_HDDADVANCED_BLANK:
				m_blank = (m_chkblank.SendMessage(BM_GETCHECK , 0 , 0) ? 1 : 0);
				return TRUE;
		}
		return FALSE;
	}

	/**
	 * CWndProc �I�u�W�F�N�g�� Windows �v���V�[�W�� (WindowProc) ���p�ӂ���Ă��܂�
	 * @param[in] nMsg ��������� Windows ���b�Z�[�W���w�肵�܂�
	 * @param[in] wParam ���b�Z�[�W�̏����Ŏg���t������񋟂��܂��B���̃p�����[�^�̒l�̓��b�Z�[�W�Ɉˑ����܂�
	 * @param[in] lParam ���b�Z�[�W�̏����Ŏg���t������񋟂��܂��B���̃p�����[�^�̒l�̓��b�Z�[�W�Ɉˑ����܂�
	 * @return ���b�Z�[�W�Ɉˑ�����l��Ԃ��܂�
	 */
	LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		return CDlgProc::WindowProc(nMsg, wParam, lParam);
	}
private:
	CComboData m_hddsize;			/*!< HDD �T�C�Y �R���g���[�� */
	UINT32 m_nHddSize;				/*!< HDD �T�C�Y */
	UINT32 m_nHddMinSize;			/*!< �ŏ��T�C�Y */
	UINT32 m_nHddMaxSize;			/*!< �ő�T�C�Y */
	
	SIZE m_szNewDisk;				//!< �E�B���h�E�̃T�C�Y
	CWndProc m_btnAdvanced;			/*!< �ڍאݒ�{�^�� */
	UINT8 m_advanced;				/*!< �ڍאݒ苖�t���O */
	UINT8 m_isSCSI;					/*!< SCSI�t���O */
	UINT32 m_HddC;					/*!< Cylinder */
	UINT16 m_HddH;					/*!< Head */
	UINT16 m_HddS;					/*!< Sector */
	UINT16 m_HddSS;					/*!< Sector Size(Bytes) */
	CComboData m_cmbhddC;			/*!< Cylinder�l �R���g���[�� */
	CComboData m_cmbhddH;			/*!< Head�l �R���g���[�� */
	CComboData m_cmbhddS;			/*!< Sector�l �R���g���[�� */
	CComboData m_cmbhddSS;			/*!< Sector Size�l �R���g���[�� */
	UINT8 m_usedynsize;				/*!< ���I���蓖�ċ��t���O */
	UINT8 m_dynsize;				/*!< ���I���蓖�ăf�B�X�N�iVHD�̂݁j */
	CWndProc m_rdbfixsize;			//!< FIXED
	CWndProc m_rdbdynsize;			//!< DYNAMIC
	UINT8 m_blank;					/*!< ��f�B�X�N�쐬�t���O */
	UINT8 m_allowsasi;				/*!< SASI�݊��`���쐬�\�t���O */
	CWndProc m_chkblank;			//!< BLANK
};



/**
 * @brief �V����HDD
 */
class CNewSasiDlg : public CDlgProc
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] hwndParent �e�E�B���h�E
	 */
	CNewSasiDlg(HWND hwndParent)
		: CDlgProc(IDD_NEWSASI, hwndParent)
		, m_nType(0)
	{
	}

	/**
	 * HDD �^�C�v�𓾂�
	 * @return HDD �^�C�v
	 */
	UINT GetType() const
	{
		return m_nType;
	}

protected:
	/**
	 * ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂�
	 * @retval TRUE �ŏ��̃R���g���[���ɓ��̓t�H�[�J�X��ݒ�
	 * @retval FALSE ���ɐݒ��
	 */
	virtual BOOL OnInitDialog()
	{
		GetDlgItem(IDC_NEWSASI5MB).SetFocus();
		return FALSE;
	}

	/**
	 * ���[�U�[�� OK �̃{�^�� (IDOK ID ���̃{�^��) ���N���b�N����ƌĂяo����܂�
	 */
	virtual void OnOK()
	{
		for (UINT i = 0; i < 6; i++)
		{
			if (IsDlgButtonChecked(s_sasires[i]) != BST_UNCHECKED)
			{
				m_nType = (i > 3) ? (i + 1) : i;
				CDlgProc::OnOK();
				break;
			}
		}
	}

private:
	UINT m_nType;			/*!< HDD �^�C�v */
};

/**
 * @brief �V����FDD
 */
class CNewFddDlg : public CDlgProc
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] hwndParent �e�E�B���h�E
	 */
	CNewFddDlg(HWND hwndParent)
		: CDlgProc((np2cfg.usefd144) ? IDD_NEWDISK2 : IDD_NEWDISK, hwndParent)
		, m_nFdType(DISKTYPE_2HD << 4)
	{
	}

	/**
	 * �^�C�v�𓾂�
	 * @return �^�C�v
	 */
	UINT8 GetType() const
	{
		return m_nFdType;
	}

	/**
	 * ���x���𓾂�
	 * @return ���x��
	 */
	LPCTSTR GetLabel() const
	{
		return m_szDiskLabel;
	}

protected:
	/**
	 * ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂�
	 * @retval TRUE �ŏ��̃R���g���[���ɓ��̓t�H�[�J�X��ݒ�
	 * @retval FALSE ���ɐݒ��
	 */
	virtual BOOL OnInitDialog()
	{
		UINT res;
		switch (m_nFdType)
		{
			case (DISKTYPE_2DD << 4):
				res = IDC_MAKE2DD;
				break;

			case (DISKTYPE_2HD << 4):
				res = IDC_MAKE2HD;
				break;

			default:
				res = IDC_MAKE144;
				break;
		}
		CheckDlgButton(res, BST_CHECKED);
		GetDlgItem(IDC_DISKLABEL).SetFocus();
		return FALSE;
	}

	/**
	 * ���[�U�[�� OK �̃{�^�� (IDOK ID ���̃{�^��) ���N���b�N����ƌĂяo����܂�
	 */
	virtual void OnOK()
	{
		GetDlgItemText(IDC_DISKLABEL, m_szDiskLabel, _countof(m_szDiskLabel));
		if (milstr_kanji1st(m_szDiskLabel, _countof(m_szDiskLabel) - 1))
		{
			m_szDiskLabel[_countof(m_szDiskLabel) - 1] = '\0';
		}
		if (IsDlgButtonChecked(IDC_MAKE2DD) != BST_UNCHECKED)
		{
			m_nFdType = (DISKTYPE_2DD << 4);
		}
		else if (IsDlgButtonChecked(IDC_MAKE2HD) != BST_UNCHECKED)
		{
			m_nFdType = (DISKTYPE_2HD << 4);
		}
		else
		{
			m_nFdType = (DISKTYPE_2HD << 4) + 1;
		}
		CDlgProc::OnOK();
	}

private:
	UINT m_nFdType;					/*!< �^�C�v */
	TCHAR m_szDiskLabel[16 + 1];	/*!< ���x�� */
};

/**
 * @brief HDD�쐬�i��
 */
class CNewHddDlgProg : public CDlgProc
{
public:
	/**
	 * �R���X�g���N�^
	 * @param[in] hwndParent �e�E�B���h�E
	 * @param[in] nProgMax �i���ő�l
	 * @param[in] nProgValue �i�����ݒl
	 */
	CNewHddDlgProg(HWND hwndParent, UINT32 nProgMax, UINT32 nProgValue)
		: CDlgProc(IDD_NEWHDDPROC, hwndParent)
	{
		SetProgressMax(nProgMax);
		SetProgressMax(nProgValue);
	}

	/**
	 * �f�X�g���N�^
	 */
	virtual ~CNewHddDlgProg()
	{
	}

	/**
	 * �v���O���X�o�[�ő�l��ݒ�
	 * @return �T�C�Y
	 */
	void SetProgressMax(UINT32 value) const
	{
		::SendMessage(GetDlgItem(IDC_HDDCREATE_PROGRESS), PBM_SETRANGE, (WPARAM)0, MAKELPARAM(0, value));
	}
	
	/**
	 * �v���O���X�o�[���ݒl��ݒ�
	 * @return �T�C�Y
	 */
	void SetProgressValue(UINT32 value) const
	{
		::SendMessage(GetDlgItem(IDC_HDDCREATE_PROGRESS), PBM_SETPOS, (WPARAM)value, 0);
	}
	
protected:
	/**
	 * ���̃��\�b�h�� WM_INITDIALOG �̃��b�Z�[�W�ɉ������ČĂяo����܂�
	 * @retval TRUE �ŏ��̃R���g���[���ɓ��̓t�H�[�J�X��ݒ�
	 * @retval FALSE ���ɐݒ��
	 */
	virtual BOOL OnInitDialog()
	{
		SetTimer(this->m_hWnd, 1, 500, NULL);
		return TRUE;
	}

	/**
	 * ���[�U�[�����j���[�̍��ڂ�I�������Ƃ��ɁA�t���[�����[�N�ɂ���ČĂяo����܂�
	 * @param[in] wParam �p�����^
	 * @param[in] lParam �p�����^
	 * @retval TRUE �A�v���P�[�V���������̃��b�Z�[�W����������
	 */
	BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}

	/**
	 * CWndProc �I�u�W�F�N�g�� Windows �v���V�[�W�� (WindowProc) ���p�ӂ���Ă��܂�
	 * @param[in] nMsg ��������� Windows ���b�Z�[�W���w�肵�܂�
	 * @param[in] wParam ���b�Z�[�W�̏����Ŏg���t������񋟂��܂��B���̃p�����[�^�̒l�̓��b�Z�[�W�Ɉˑ����܂�
	 * @param[in] lParam ���b�Z�[�W�̏����Ŏg���t������񋟂��܂��B���̃p�����[�^�̒l�̓��b�Z�[�W�Ɉˑ����܂�
	 * @return ���b�Z�[�W�Ɉˑ�����l��Ԃ��܂�
	 */
	LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (nMsg) {
		case WM_DESTROY:
			KillTimer(this->m_hWnd, 1);
			break;
		case WM_TIMER:
			SetProgressValue(mt_progressvalue);
			SetProgressMax(mt_progressmax);
			if(mt_progressvalue >= mt_progressmax){
				// �����I���
				CDlgProc::OnOK();
			}
			return 0;
		}
		return CDlgProc::WindowProc(nMsg, wParam, lParam);
	}
private:
};

static HANDLE	newdisk_hThread = NULL; // �f�B�X�N�쐬�p�X���b�h
static int _mt_cancel = 0;
static int _mt_dyndisk = 0;
static int _mt_blank = 0;
static TCHAR _mt_lpPath[MAX_PATH] = {0};
static UINT32 _mt_diskSize = 0;
static UINT32 _mt_diskC = 0;
static UINT16 _mt_diskH = 0;
static UINT16 _mt_diskS = 0;
static UINT16 _mt_diskSS = 0;

static unsigned int __stdcall newdisk_ThreadFunc(LPVOID vdParam)
{
	LPCTSTR lpPath = _mt_lpPath;
	LPCTSTR ext = file_getext(lpPath);
	if (!file_cmpname(ext, str_thd))
	{
		newdisk_thd(lpPath, _mt_diskSize);
	}
	else if (!file_cmpname(ext, str_nhd))
	{
		if(_mt_diskSize){
			// �S�e�ʎw�胂�[�h
			newdisk_nhd_ex(lpPath, _mt_diskSize, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}else{
			// CHS�w�胂�[�h
			newdisk_nhd_ex_CHS(lpPath, _mt_diskC, _mt_diskH, _mt_diskS, _mt_diskSS, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}
	}
	else if (!file_cmpname(ext, str_hdi))
	{
		if(_mt_diskSize){
			// �S�e�ʎw�胂�[�h
			newdisk_hdi_ex(lpPath, _mt_diskSize, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}else{
			// CHS�w�胂�[�h
			newdisk_hdi_ex_CHS(lpPath, _mt_diskC, _mt_diskH, _mt_diskS, _mt_diskSS, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}
	}
#if defined(SUPPORT_SCSI)
	else if (!file_cmpname(ext, str_hdd))
	{
		newdisk_vhd(lpPath, _mt_diskSize);
	}
	else if (!file_cmpname(ext, str_hdn))
	{
		newdisk_hdn(lpPath, _mt_diskSize);
	}
#endif
#ifdef SUPPORT_VPCVHD
	else if (!file_cmpname(ext, str_vhd))
	{
		if(_mt_diskSize){
			// �S�e�ʎw�胂�[�h
			newdisk_vpcvhd_ex(lpPath, _mt_diskSize, _mt_dyndisk, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}else{
			// CHS�w�胂�[�h
			newdisk_vpcvhd_ex_CHS(lpPath, _mt_diskC, _mt_diskH, _mt_diskS, _mt_diskSS, _mt_dyndisk, _mt_blank, &mt_progressvalue, &_mt_cancel);
		}
	}
#endif
	mt_progressvalue = mt_progressmax;
	return 0;
}

/**
 * �V�K�f�B�X�N�쐬 �_�C�A���O
 * @param[in] hWnd �e�E�B���h�E
 */
void dialog_newdisk_ex(HWND hWnd, int mode)
{
	unsigned int dwID;
	TCHAR szPath[MAX_PATH];
	std::tstring rTitle;
	std::tstring rDefExt;
	std::tstring rFilter;
	if(mode == NEWDISKMODE_HD){
		file_cpyname(szPath, hddfolder, _countof(szPath));
		file_cutname(szPath);
		file_catname(szPath, str_newdisk, _countof(szPath));
		rTitle = std::tstring(LoadTString(IDS_NEWDISKTITLE));
		rDefExt = std::tstring(OEMTEXT("nhd"));
#if defined(SUPPORT_SCSI)
		rFilter = std::tstring(LoadTString(IDS_NEWDISKHDFILTER));
#else	// defined(SUPPORT_SCSI)
		rFilter = std::tstring(LoadTString(IDS_NEWDISKHDFILTER2));
#endif	// defined(SUPPORT_SCSI)
	}else if(mode == NEWDISKMODE_FD){
		file_cpyname(szPath, fddfolder, _countof(szPath));
		file_cutname(szPath);
		file_catname(szPath, str_newdisk, _countof(szPath));
		rTitle = std::tstring(LoadTString(IDS_NEWDISKTITLE));
		rDefExt = std::tstring(LoadTString(IDS_NEWDISKEXT));
		rFilter = std::tstring(LoadTString(IDS_NEWDISKFDFILTER));
	}else{
		file_cpyname(szPath, fddfolder, _countof(szPath));
		file_cutname(szPath);
		file_catname(szPath, str_newdisk, _countof(szPath));
		rTitle = std::tstring(LoadTString(IDS_NEWDISKTITLE));
		rDefExt = std::tstring(LoadTString(IDS_NEWDISKEXT));
#if defined(SUPPORT_SCSI)
		rFilter = std::tstring(LoadTString(IDS_NEWDISKFILTER));
#else	// defined(SUPPORT_SCSI)
		rFilter = std::tstring(LoadTString(IDS_NEWDISKFILTER2));
#endif	// defined(SUPPORT_SCSI)
	}


	CFileDlg fileDlg(FALSE, rDefExt.c_str(), szPath, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, rFilter.c_str(), hWnd);
	fileDlg.m_ofn.lpstrTitle = rTitle.c_str();
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}

	LPCTSTR lpPath = fileDlg.GetPathName();
	LPCTSTR ext = file_getext(lpPath);
	if (!file_cmpname(ext, str_thd))
	{
		CNewHddDlg dlg(hWnd, 5, 256, 1);
		if (dlg.DoModal() == IDOK)
		{
			newdisk_thd(lpPath, dlg.GetSize());
		}
	}
	else if (!file_cmpname(ext, str_nhd))
	{
		CNewHddDlg dlg(hWnd, 5, np2oscfg.makelhdd ? NHD_MAXSIZE2 : NHD_MAXSIZE, 1);
		dlg.EnableAdvancedOptions();
		if (dlg.DoModal() == IDOK)
		{
			//if(dlg.IsAdvancedMode()){
				_mt_diskSize = 0;
				_mt_diskC = dlg.GetC();
				_mt_diskH = dlg.GetH();
				_mt_diskS = dlg.GetS();
				_mt_diskSS = dlg.GetSS();
				if ((_mt_diskC > 65535 || _mt_diskH > 16 || _mt_diskS > 255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
					return;
				}
			//}else{
			//	_mt_diskSize = dlg.GetSize();
			//	if (((FILELEN)_mt_diskSize * _mt_diskH * _mt_diskS > 65535*16*255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
			//		return;
			//	}
			//}
			_mt_blank = dlg.IsBlankDisk();
			_mt_dyndisk = 0;
			_mt_cancel = 0;
			mt_progressvalue = 0;
			mt_progressmax = 100;
			_tcscpy(_mt_lpPath, lpPath);
			newdisk_hThread = (HANDLE)_beginthreadex(NULL , 0 , newdisk_ThreadFunc  , NULL , 0 , &dwID);
			CNewHddDlgProg dlg2(hWnd, mt_progressmax, mt_progressvalue);
			if (dlg2.DoModal() != IDOK)
			{
				_mt_cancel = 1;
				WaitForSingleObject(newdisk_hThread, INFINITE);
				CloseHandle(newdisk_hThread);
			}
			_mt_cancel = 1;
		}
	}
	else if (!file_cmpname(ext, str_hdi))
	{
		CNewHddDlg dlg(hWnd, 5, np2oscfg.makelhdd ? NHD_MAXSIZE2 : NHD_MAXSIZE, 1);
		dlg.EnableAdvancedOptions();
		if (dlg.DoModal() == IDOK)
		{
			//if(dlg.IsAdvancedMode()){
				_mt_diskSize = 0;
				_mt_diskC = dlg.GetC();
				_mt_diskH = dlg.GetH();
				_mt_diskS = dlg.GetS();
				_mt_diskSS = dlg.GetSS();
				if ((_mt_diskC > 65535 || _mt_diskH > 16 || _mt_diskS > 255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
					return;
				}
			//}else{
			//	_mt_diskSize = dlg.GetSize();
			//	if (((FILELEN)_mt_diskSize * _mt_diskH * _mt_diskS > 65535*16*255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
			//		return;
			//	}
			//}
			_mt_blank = dlg.IsBlankDisk();
			_mt_dyndisk = 0;
			_mt_cancel = 0;
			mt_progressvalue = 0;
			mt_progressmax = 100;
			_tcscpy(_mt_lpPath, lpPath);
			newdisk_hThread = (HANDLE)_beginthreadex(NULL , 0 , newdisk_ThreadFunc  , NULL , 0 , &dwID);
			CNewHddDlgProg dlg2(hWnd, mt_progressmax, mt_progressvalue);
			if (dlg2.DoModal() != IDOK)
			{
				_mt_cancel = 1;
				WaitForSingleObject(newdisk_hThread, INFINITE);
				CloseHandle(newdisk_hThread);
			}
			_mt_cancel = 1;
		}
	}
#if defined(SUPPORT_SCSI)
	else if (!file_cmpname(ext, str_hdd))
	{
		CNewHddDlg dlg(hWnd, 2, 512, 1);
		if (dlg.DoModal() == IDOK)
		{
			newdisk_vhd(lpPath, dlg.GetSize());
		}
	}
	else if (!file_cmpname(ext, str_hdn))
	{
		CNewHddDlg dlg(hWnd, 2, 6399, 1);
		if (dlg.DoModal() == IDOK)
		{
			newdisk_hdn(lpPath, dlg.GetSize());
		}
	}
#endif
#ifdef SUPPORT_VPCVHD
	else if (!file_cmpname(ext, str_vhd))
	{
		CNewHddDlg dlg(hWnd, 5, np2oscfg.makelhdd ? NHD_MAXSIZE2 : NHD_MAXSIZE, 0);
		dlg.EnableAdvancedOptions();
		dlg.EnableDynamicSize();
		if (dlg.DoModal() == IDOK)
		{
			//if(dlg.IsAdvancedMode()){
				_mt_diskSize = 0;
				_mt_diskC = dlg.GetC();
				_mt_diskH = dlg.GetH();
				_mt_diskS = dlg.GetS();
				_mt_diskSS = dlg.GetSS();
				if ((_mt_diskC > 65535 || _mt_diskH > 16 || _mt_diskS > 255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
					return;
				}
			//}else{
			//	_mt_diskSize = dlg.GetSize();
			//	if (((FILELEN)_mt_diskSize * _mt_diskH * _mt_diskS > 65535*16*255) && MessageBoxA(hWnd, "Specified parameters are incompatible with CHS access.\nDo you want to continue anyway?", "New Disk", MB_YESNO | MB_ICONQUESTION) != IDYES) {
			//		return;
			//	}
			//}
			_mt_blank = dlg.IsBlankDisk();
			_mt_dyndisk = dlg.IsDynamicDisk();
			_mt_cancel = 0;
			mt_progressvalue = 0;
			mt_progressmax = 100;
			_tcscpy(_mt_lpPath, lpPath);
			newdisk_hThread = (HANDLE)_beginthreadex(NULL , 0 , newdisk_ThreadFunc  , NULL , 0 , &dwID);
			CNewHddDlgProg dlg2(hWnd, mt_progressmax, mt_progressvalue);
			if (dlg2.DoModal() != IDOK)
			{
				_mt_cancel = 1;
				WaitForSingleObject(newdisk_hThread, INFINITE);
				CloseHandle(newdisk_hThread);
			}
			_mt_cancel = 1;
		}
	}
#endif
	else if ((!file_cmpname(ext, str_d88)) ||
			(!file_cmpname(ext, str_d98)) ||
			(!file_cmpname(ext, str_88d)) ||
			(!file_cmpname(ext, str_98d)))
	{
		CNewFddDlg dlg(hWnd);
		if (dlg.DoModal()  == IDOK)
		{
			newdisk_fdd(lpPath, dlg.GetType(), dlg.GetLabel());
		}
	}
	else if (!file_cmpname(ext, str_hdm))
	{
		newdisk_123mb_fdd(lpPath);
	}
	else if (!file_cmpname(ext, str_hd4))
	{
		newdisk_144mb_fdd(lpPath);
	}
}

/**
 * �V�K�f�B�X�N�쐬 �_�C�A���O
 * @param[in] hWnd �e�E�B���h�E
 */
void dialog_newdisk(HWND hWnd)
{
	dialog_newdisk_ex(hWnd, NEWDISKMODE_ALL);
}


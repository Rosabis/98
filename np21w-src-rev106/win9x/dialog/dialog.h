/**
 * @file	dialog.h
 * @breif	�_�C�A���O�̐錾
 */

#pragma once

enum {
	NEWDISKMODE_ALL = 0,
	NEWDISKMODE_FD = 1,
	NEWDISKMODE_HD = 2,
};

// d_about.cpp
void dialog_about(HWND hwndParent);

// d_bmp.cpp
void dialog_writebmp(HWND hWnd);

// d_txt.cpp
void dialog_writetxt(HWND hWnd);
void dialog_getTVRAM(OEMCHAR *buffer);

// d_cfgload.cpp
int  dialog_readnpcfg(HWND hWnd);

// d_cfgsave.cpp
void dialog_writenpcfg(HWND hWnd);

// d_clnd.cpp
void dialog_calendar(HWND hwndParent);

// d_config.cpp
void dialog_configure(HWND hwndParent);

// d_disk.cpp
void dialog_changefdd(HWND hWnd, REG8 drv);
void dialog_changehdd(HWND hWnd, REG8 drv);
void dialog_newdisk_ex(HWND hWnd, int mode);
void dialog_newdisk(HWND hWnd);
#if defined(SUPPORT_MOUNTDIR)
void dialog_changedir(HWND hWnd, REG8 drv);
#endif

// d_font.cpp
void dialog_font(HWND hWnd);

// d_mpu98.cpp
void dialog_mpu98(HWND hwndParent);

// d_screen.cpp
void dialog_scropt(HWND hwndParent);

// d_serial.cpp
void dialog_serial(HWND hWnd);

// d_sound.cpp
void dialog_sndopt(HWND hwndParent);

// d_soundlog.cpp
void dialog_soundlog(HWND hWnd);

// d_network.cpp
#if defined(SUPPORT_NET)
void dialog_netopt(HWND hWnd);
#endif

// d_wab.cpp
#if defined(SUPPORT_CL_GD5430)
void dialog_wabopt(HWND hWnd);
#endif

// d_ide.cpp
#if defined(SUPPORT_IDEIO)
void dialog_ideopt(HWND hwndParent);
#endif

// d_hostdrv.cpp
#if defined(SUPPORT_HOSTDRV)
void dialog_hostdrvopt(HWND hwndParent);
void hostdrv_readini(); // �b�� hostdrv.c������Ɉړ����ׂ��H
void hostdrv_writeini(); // �b�� hostdrv.c������Ɉړ����ׂ��H
#endif

// d_pci.cpp
#if defined(SUPPORT_PCI)
void dialog_pciopt(HWND hWnd);
#endif

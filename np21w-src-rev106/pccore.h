/**
 * @file	pccore.h
 * @brief	Interface of the core
 */

#pragma once

#include "nevent.h"

#define MAX_SASI 4
#define MAX_SCSI 4
#define MAX_DIR 3

enum {
	PCBASECLOCK25		= 2457600,
	PCBASECLOCK20		= 1996800
};

enum {
	CPUMODE_8MHZ		= 0x20,

	PCMODEL_VF			= 0,
	PCMODEL_VM			= 1,
	PCMODEL_VX			= 2,
	PCMODELMASK			= 0x3f,
	PCMODEL_PC9821		= 0x40,
	PCMODEL_EPSON		= 0x80,

	PCHDD_SASI			= 0x01,
	PCHDD_SCSI			= 0x02,
	PCHDD_IDE			= 0x04,

	PCROM_BIOS			= 0x01,
	PCROM_SOUND			= 0x02,
	PCROM_SASI			= 0x04,
	PCROM_SCSI			= 0x08,
	PCROM_BIOS9821		= 0x10,

	PCCBUS_PC9861K		= 0x0001,
	PCCBUS_MPU98		= 0x0002,
	PCCBUS_SMPU98		= 0x0004
};

/**
 * Sound ID
 */
enum tagSoundId
{
	SOUNDID_NONE				= 0,		/*!< No boards */
	SOUNDID_PC_9801_14			= 0x01,		/*!< PC-9801-14 */ 
	SOUNDID_PC_9801_26K			= 0x02,		/*!< PC-9801-26K */ 
	SOUNDID_PC_9801_86			= 0x04,		/*!< PC-9801-86 */ 
	SOUNDID_PC_9801_86_26K		= 0x06,		/*!< PC-9801-86 + 26K */ 
	SOUNDID_PC_9801_118			= 0x08,		/*!< PC-9801-118 */
	SOUNDID_PC_9801_86_ADPCM	= 0x14,		/*!< PC-9801-86 with ADPCM */
	SOUNDID_SPEAKBOARD			= 0x20,		/*!< Speak board */
	SOUNDID_86_SPEAKBOARD		= 0x24,		/*!< PC-9801-86 + Speak board */
	SOUNDID_SPARKBOARD			= 0x40,		/*!< Spark board */
	SOUNDID_SB16				= 0x41,		/*!< Sound Blaster 16 */
	SOUNDID_PC_9801_86_WSS_SB16	= 0x42,		/*!< PC-9801-86 + Mate-X PCM(B460) + Sound Blaster 16 */
	SOUNDID_WSS_SB16			= 0x43,		/*!< Mate-X PCM(B460) + Sound Blaster 16 */
	SOUNDID_PC_9801_86_SB16		= 0x44,		/*!< PC-9801-86 + Sound Blaster 16 */
	SOUNDID_PC_9801_118_SB16	= 0x45,		/*!< PC-9801-118 + Sound Blaster 16 */
	SOUNDID_PC_9801_86_118_SB16 = 0x46,		/*!< PC-9801-86 + PC-9801-118(B460) + Sound Blaster 16 */
	SOUNDID_PC_9801_26K_SB16	= 0x47,		/*!< PC-9801-26K + Sound Blaster 16 (YM2203 Upgrade Kit) */
	SOUNDID_MATE_X_PCM			= 0x60,		/*!< Mate-X PCM */
	SOUNDID_PC_9801_86_WSS		= 0x64,		/*!< PC-9801-86 + Mate-X PCM(B460) */
	SOUNDID_PC_9801_86_118		= 0x68,		/*!< PC-9801-86 + PC-9801-118(B460) */
	SOUNDID_WAVESTAR			= 0x70,		/*!< Wave Star */
	SOUNDID_AMD98				= 0x80,		/*!< AMD-98 */
	SOUNDID_SOUNDORCHESTRA		= 0x32,		/*!< SOUND ORCHESTRA */
	SOUNDID_SOUNDORCHESTRAV		= 0x82,		/*!< SOUND ORCHESTRA-V */
	SOUNDID_MULTIMEDIAORCHESTRA = 0x83,		/*!< Multimedia Orchestra */

#if defined(SUPPORT_PX)
	SOUNDID_PX1					= 0x30,
	SOUNDID_PX2					= 0x50,
#endif	/* defined(SUPPORT_PX) */
	
	SOUNDID_INVALID				= 0xff,		/*!< for new statesave checker */
};
typedef enum tagSoundId		SOUNDID;

enum {
	FPU_TYPE_SOFTFLOAT	= 0, /* Berkeley SoftFloat */
	FPU_TYPE_DOSBOX		= 1, /* DOSBox FPU */
	FPU_TYPE_DOSBOX2	= 2  /* DOSBox FPU+INT64 */
};

#if defined(SUPPORT_WAB_NPDISP)
enum {
	NPDISP_ACCEL_LEGACY		= 0,
	NPDISP_ACCEL_DIRECTDRAW	= 1,
	NPDISP_ACCEL_DIRECT3D		= 2
};

enum {
	NPDISP_D3D_QUALITY_ACCURATE = 0,
	NPDISP_D3D_QUALITY_POINT = 1,
	NPDISP_D3D_QUALITY_AFFINE = 2,
	NPDISP_D3D_QUALITY_FASTEST = 3,
	NPDISP_D3D_QUALITY_COARSE2X2 = 4,
	NPDISP_D3D_QUALITY_MAX = NPDISP_D3D_QUALITY_COARSE2X2,
	NPDISP_D3D_QUALITY_FAST_TNL = NPDISP_D3D_QUALITY_FASTEST
};
#endif

/**
 * @brief config
 */
struct tagNP2Config
{
	// �G�~�����[�g���ɂ悭�Q�Ƃ����z
	UINT8	uPD72020;
	UINT8	DISPSYNC;
	UINT8	RASTER;
	UINT8	realpal;
	UINT8	LCD_MODE;
	UINT8	skipline;
	UINT16	skiplight;

	UINT8	KEY_MODE;
	UINT8	XSHIFT;
	UINT8	BTN_RAPID;
	UINT8	BTN_MODE;

	UINT8	dipsw[3];
	UINT8	MOUSERAPID;

	UINT8	calendar;
	UINT8	usefd144;
	UINT8	wait[6];
	
	UINT8	davolume;
	UINT8	modelnum;

	UINT8	timerfix;
	
#if defined(SUPPORT_ASYNC_CPU)
	UINT8	asynccpu; // �񓯊�CPU���[�h�L��
	UINT8	asynclvl; // �񓯊�CPU���[�h�������x���i0:����Ŏ�`100:�A�O���b�V�u�j
#endif
	UINT8	consttsc; // RDTSC��Async�N���b�N�ύX�ɂ�炸���Ԋu�ɂ���
#if defined(SUPPORT_IDEIO)
	UINT8	idebaddr; // IDE BIOS �A�h���X�i�f�t�H���g�FD8h(D8000h)�j
#endif
	
	// ���Z�b�g���Ƃ�����܂�Q�Ƃ���Ȃ��z
	OEMCHAR	model[8];
	UINT	baseclock;
	UINT	multiple;
	
	UINT8	usebios;

	UINT8	memsw[8];

	UINT8	ITF_WORK;
#if defined(SUPPORT_LARGE_MEMORY)
	UINT16	EXTMEM;
#else
	UINT8	EXTMEM;
#endif
	UINT8	grcg;
	UINT8	color16;
	UINT32	BG_COLOR;
	UINT32	FG_COLOR;

	UINT32	samplingrate;
	UINT16	delayms;
	UINT8	SOUND_SW;
	UINT8	snd_x;

	UINT8	snd14opt[3];
	UINT8	snd26opt;
	UINT8	snd86opt;
	UINT8	spbopt;
	UINT8	spb_vrc;												// ver0.30
	UINT8	spb_vrl;												// ver0.30
	UINT8	spb_x;													// ver0.30

	UINT16	snd118io;
	UINT8	snd118id;
	UINT8	snd118dma;
	UINT8	snd118irqf;
	UINT8	snd118irqp;
	UINT8	snd118irqm;
	UINT8	snd118rom;
	
	UINT8	sndwssid;
	UINT8	sndwssdma;
	UINT8	sndwssirq;

	UINT16	sndwsio;
	UINT8	sndwsdma;
	UINT8	sndwsirq;
	UINT8	sndwspnp;
	UINT8	sndwspcm;
	
#if defined(SUPPORT_SOUND_SB16)
	UINT8	sndsb16io;
	UINT8	sndsb16dma;
	UINT8	sndsb16irq;
	UINT8	sndsb16at;
#endif	/* SUPPORT_SOUND_SB16 */

	UINT8	BEEP_VOL;
	UINT8	vol14[6];
	UINT8	vol_master;
	UINT8	vol_fm;
	UINT8	vol_ssg;
	UINT8	vol_adpcm;
	UINT8	vol_pcm;
	UINT8	vol_rhythm;
	UINT8	vol_midi;

	UINT8	mpuenable;
	UINT8	mpuopt;
	UINT8	mpu_at;
	
#if defined(SUPPORT_SMPU98)
	UINT8	smpuenable;
	UINT8	smpuopt;
	UINT8	smpumuteB;
#endif	/* SUPPORT_SMPU98 */

	UINT8	pc9861enable;
	UINT8	pc9861sw[3];
	UINT8	pc9861jmp[6];
	
#if defined(SUPPORT_FMGEN)
	UINT8	usefmgen;
#endif	/* SUPPORT_FMGEN */

	UINT8	fddequip;
	UINT8	MOTOR;
	UINT8	MOTORVOL;
#if defined(SUPPORT_FDDSNDDEV)
	OEMCHAR	fddSndDevice[MAX_PATH];		/* FDD seek sound output device ("" = main) */
	OEMCHAR	boardSndDevice[MAX_PATH];	/* board(relay) sound output device ("" = main) */
	UINT16	fddSndVol;					/* FDD seek separate-output volume (0-100) */
	UINT16	boardSndVol;				/* board(relay) separate-output volume (0-100) */
#endif
	UINT8	PROTECTMEM;
	UINT8	hdrvacc;
	UINT8	hdrvenable;
	UINT8	hdrvntenable;
	
	UINT8	savefddfile;											// ver0.86w rev20
	OEMCHAR	fddfile[4][MAX_PATH];									// ver0.86w rev20

#if defined(SUPPORT_IDEIO)
	OEMCHAR	sasihdd[4][MAX_PATH];									// ver0.86w
	UINT8	idetype[4];												// ver0.86w
	OEMCHAR	idecd[4][MAX_PATH];										// ver0.85w
	UINT8	idebios;												// ver0.86w rev20
	UINT8	autoidebios;												// ver0.86w rev36
	UINT32	iderwait; // IDE�ǂݎ��̊��荞�ݒx������(clock)�B  np21w ver0.86 rev19
	UINT32	idewwait; // IDE�������݂̊��荞�ݒx������(clock)�B  np21w ver0.86 rev18
	UINT32	idemwait; // IDE BIOS������ꍇ�̊��荞�ݒx���ŏ��l  np21w ver0.86 rev26 �p�~
	UINT8	savecdfile;	
	UINT8	useasynccd;
	UINT8	allowcdtraycmd;	
	UINT8	useasynchd;
#else
	OEMCHAR	sasihdd[2][MAX_PATH];									// ver0.74
#endif
#if defined(SUPPORT_SCSI)
	OEMCHAR	scsihdd[4][MAX_PATH];									// ver0.74
#endif
	OEMCHAR	mountdir[MAX_DIR][MAX_PATH];
	OEMCHAR	fontfile[MAX_PATH];
	OEMCHAR	biospath[MAX_PATH];
	OEMCHAR	hdrvroot[MAX_PATH];
	
#ifdef SUPPORT_NET
	OEMCHAR	np2nettap[MAX_PATH];
	UINT8	np2netpmm;
#endif
#ifdef SUPPORT_LGY98
	UINT8	uselgy98;
	UINT16	lgy98io;
	UINT8	lgy98irq;
	UINT8	lgy98mac[6];
#endif
#ifdef SUPPORT_WAB
	UINT8	wabasw;
#endif
#ifdef SUPPORT_CL_GD5430
	UINT8	usegd5430;
	UINT16	gd5430type;
	UINT8	gd5430fakecur;
	UINT8	gd5430melofs;
	UINT8	ga98nb_bigscrn_ex;
#endif
#if defined(SUPPORT_WAB_NPDISP)
	UINT8	usenpdisp;
	UINT8	npdispaccel;
	UINT8	npdisptnlenable;
	UINT8	npdisptnlfast;		// legacy INI compatibility; use npdispd3dquality for new settings
	UINT8	npdispd3dquality;
#endif
#if defined(SUPPORT_WAB_GA1280A)
	UINT8	usega1280a;
#endif
#if defined(SUPPORT_VGA_MODEX)
	UINT8	usemodex;
#endif
#if defined(SUPPORT_GPIB)
	UINT8	usegpib; // GPIB�g�p
	UINT8	gpibirq; // GPIB IRQ
	UINT8	gpibmode; // GPIB Master/Slave
	UINT8	gpibaddr; // GPIB Address
	UINT8	gpibexio; // GPIB custom I/O port base
#endif
#if defined(SUPPORT_PCI)
	UINT8	usepci; // PCI Bus�g�p
	UINT8	pci_bios32; // BIOS32�g�p
	UINT8	pci_pcmc; // PCMC�I��
#endif
	
	UINT8	winntfix;
	UINT16	sysiomsk;
	
	UINT8	memchkmx;
	UINT8	sbeeplen;
	UINT8	sbeepadj;
	
	UINT8	biosioemu;

	char	cpu_vendor[16]; // �x���_�[�i12byte�j
	UINT32	cpu_family; // �t�@�~��
	UINT32	cpu_model; // ���f��
	UINT32	cpu_stepping; // �X�e�b�s���O
	UINT32	cpu_feature; // �@�\�t���O
	UINT32	cpu_feature_ex; // �g���@�\�t���O
	char	cpu_brandstring[64]; // �u�����h���i48byte�j
	OEMCHAR	cpu_vendor_o[16]; // �x���_�[�i12byte�jOEMCHAR
	OEMCHAR	cpu_brandstring_o[64]; // �u�����h���i48byte�jOEMCHAR
	UINT32	cpu_brandid; // �u�����hID
	UINT32  cpu_feature_ecx; // ECX�@�\�t���O
	UINT32  cpu_feature_ex_ecx; // ECX�g���@�\�t���O
	UINT32  cpu_eflags_mask; // EFLAGS�}�X�N
	
	UINT8	fpu_type; // FPU��ށi0=Berkeley SoftFloat, 1=DOSBox FPU, 2=DOSBox FPU+INT64�j
	
#if defined(SUPPORT_FAST_MEMORYCHECK)
	UINT8	memcheckspeed; // �������`�F�b�N���x
#endif
	
	UINT8	useram_d; // EPSON�łȂ��Ă�D0000h-DFFFFh��RAM�Ɂi������IDE BIOS D8000h-DBFFFh�͑ʖځj
	UINT8	usepegcplane; // PEGC �v���[�����[�h�T�|�[�g
	
	UINT8	usecdecc; // CD-ROM EDC/ECC �G�~�����[�V�����T�|�[�g
	UINT8	cddtskip; // CD-ROM �I�[�f�B�I�Đ����Ƀf�[�^�g���b�N���X�L�b�v

#if defined(SUPPORT_GAMEPORT)
	UINT8	gameport; // SB16��118�����̃Q�[���|�[�g���g�p����
	UINT8	analogjoy; // �Q�[���|�[�g���A�i���O�W���C�X�e�B�b�N�ɂ���
#endif
	UINT8	allowMOVCS; // mov cs,xx���߂̎��s��������i8086�j
	UINT8	usetexthook; // Text Hook��L���ɂ���i�C�O�����j
	UINT8	rascsi92; // 92�݊���RaSCSI�`��(*.HDN)��ǂ�
	UINT8	nbeepofs; // BEEP�̏o�̓I�t�Z�b�g������

	SINT64	cal_vofs; // ���z�J�����_�����I�t�Z�b�g(�b)

#if defined(SUPPORT_NP2SCSI)
	UINT8	usenp2stor; // np2 SCSI���g�p����
#endif

	UINT32	emuspeed; // emulation speed(%)

	OEMCHAR	fontface[256];

	UINT8	slowmous; // 4�r�b�g�؂�̂Ă̒ᑬ�}�E�X���g��
	UINT8	gdcwovrg; // 98�W���O���t�B�b�N�ŉ�ʕ�80��������F�߂�
};
typedef struct tagNP2Config  NP2CFG;		/*!< The define of config */

typedef struct {
	UINT32	baseclock;
	UINT	multiple;

	UINT8	cpumode;
	UINT8	model;
	UINT8	hddif;
#if defined(SUPPORT_LARGE_MEMORY)
	UINT16	extmem;
#else
	UINT8	extmem;
#endif
	UINT8	dipsw[3];		// ���Z�b�g����DIPSW
	UINT8	rom;

	SOUNDID sound;
	UINT32	device;

	UINT32	realclock;

	UINT	maxmultiple;
} PCCORE;

enum {
	COREEVENT_SHUT		= 0,
	COREEVENT_RESET		= 1,
	COREEVENT_EXIT		= 2
};

typedef struct
{
	UINT8	screenupdate;
	UINT8	screendispflag;
	UINT8	drawframe;
	UINT8	hardwarereset;
} PCSTAT;

#if defined(SUPPORT_ASYNC_CPU)
#define ASYNCCPU_CLOCKTABLE_MAX	64
typedef struct
{
	UINT32 drawskip;
	UINT32 nowait;
	UINT32 lastTimingValue;
	UINT32 asyncTarget;
	int lastTimingValid;
	int screendisp;
	int threshold_down;
	int threshold_up;
	int clockUpTbl[ASYNCCPU_CLOCKTABLE_MAX];
	int clockDownTbl[ASYNCCPU_CLOCKTABLE_MAX];
} ASYNCCPUSTAT;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const OEMCHAR np2version[];

extern	NP2CFG	np2cfg;
extern	PCCORE	pccore;
extern	PCSTAT	pcstat;
extern	UINT8	soundrenewal;
extern	UINT	drawcount;

#if defined(SUPPORT_ASYNC_CPU)
extern	ASYNCCPUSTAT	pccore_asynccpustat;
#endif

void getbiospath(OEMCHAR *path, const OEMCHAR *fname, int maxlen);
void screendisp(NEVENTITEM item);
void screenvsync(NEVENTITEM item);


void pccore_cfgupdate(void);

#if defined(SUPPORT_IA32_HAXM)
void pccore_mem_malloc(void);
void pccore_mem_free(void);
#endif
#if defined(SUPPORT_IA32_WHPX)
void pccore_mem_malloc(void);
void pccore_mem_free(void);
#endif
void pccore_init(void);
void pccore_term(void);
void pccore_reset(void);
void pccore_exec(BOOL draw);

void pccore_postevent(UINT32 event);

#ifdef SUPPORT_ASYNC_CPU
void pccore_asynccpu_initialize(void);
void pccore_asynccpu_updatesettings(int asyncLevel);

extern int asynccpu_lateflag;
extern int asynccpu_fastflag;
#if !defined(__LIBRETRO__) && !defined(NP2_SDL2) && !defined(NP2_X11)
extern LARGE_INTEGER asynccpu_lastclock;
extern LARGE_INTEGER asynccpu_clockpersec;
extern LARGE_INTEGER asynccpu_clockcount;
#else
typedef union {
    struct {
        UINT32 LowPart;
        SINT32 HighPart;
    } u;
    SINT64 QuadPart;
} LARGE_INTEGER;
extern LARGE_INTEGER asynccpu_lastclock;
extern LARGE_INTEGER asynccpu_clockpersec;
extern LARGE_INTEGER asynccpu_clockcount;
#endif
#endif

#ifdef __cplusplus
}
#endif


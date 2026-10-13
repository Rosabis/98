
#if !defined(SUPPORT_PC9821)
#define PROJECTNAME			"Neko Project II/W"
#else
#define PROJECTNAME			"Neko Project 21/W"
#endif

#if !defined(_WIN64)
#define PROJECTSUBNAME		""
#else
#define PROJECTSUBNAME		" x64"
#endif

enum {
	WHEELACTION_NONE = 0,
	WHEELACTION_VOLUME = 1,
	WHEELACTION_MOUSESPEED = 2,
	WHEELACTION_UPDOWN = 3,
	WHEELACTION_ROLLUPDOWN = 4,
};

typedef struct {
	UINT8	port;
	UINT8	def_en;
	UINT8	param;
	UINT32	speed;
	OEMCHAR	mout[MAXPNAMELEN];
	OEMCHAR	min[MAXPNAMELEN];
	OEMCHAR	mdl[64];
	OEMCHAR	def[MAX_PATH];
	UINT8	fixedspeed;
	UINT8	DSRcheck;
	OEMCHAR	dirpath[MAX_PATH]; // Path to the output directory for dump files
	UINT32	fileTimeout;
#if defined(SUPPORT_NAMED_PIPE)
	OEMCHAR	pipename[MAX_PATH]; // The name of the named-pipe
	OEMCHAR	pipeserv[MAX_PATH]; // The server name of the named-pipe
#endif
	OEMCHAR	spoolPrinterName[MAX_PATH];
	UINT32	spoolTimeout;
	UINT8	spoolEmulation;
	UINT8	spoolDotSize;
	UINT8	spoolRectDot;
	UINT8	spoolPageAlignment;
	UINT32	spoolOffsetXmm;
	UINT32	spoolOffsetYmm;
	UINT32	spoolScale;
} COMCFG;

typedef struct {
	OEMCHAR	titles[256];
	OEMCHAR	winid[4];

	int		winx;
	int		winy;
	UINT	paddingx;
	UINT	paddingy;
	UINT	fscrn_cx;
	UINT	fscrn_cy;
	UINT8	force400;
	UINT8	WINSNAP;
	UINT8	NOWAIT;
	UINT8	DRAW_SKIP;

	UINT8	background;
	UINT8	DISPCLK;
	UINT8	KEYBOARD;
	UINT8	F12COPY;
	UINT8	USENUMLOCK;

	UINT8	MOUSE_SW;
	UINT8	JOYPAD1;
	UINT8	JOYPAD2;
	UINT8	JOY1BTN[4];
	UINT8	JOY2BTN[4];
	UINT8	JOYPAD1ID;
	UINT8	JOYPAD2ID;
	UINT8	JOYPAD1POVXY;
	UINT8	JOYPAD2POVXY;

	COMCFG	mpu;
#if defined(SUPPORT_SMPU98)
	COMCFG	smpuA;
	COMCFG	smpuB;
#endif
	COMCFG	com1;
	COMCFG	com2;
	COMCFG	com3;
	COMCFG	lpt1;

	UINT32	clk_color1;
	UINT32	clk_color2;
	UINT8	clk_x;
	UINT8	clk_fnt;

	UINT8	comfirm;
	UINT8	shortcut;												// ver0.30

	UINT8	resume;													// ver0.30
	UINT8	statsave;
#if !defined(_WIN64)
	UINT8	disablemmx;
#endif
	UINT8	wintype;
	UINT8	toolwin;
	UINT8	keydisp;
	UINT8	skbdwin;
	UINT8	I286SAVE;
	UINT8	hostdrv_write;
	UINT8	jastsnd;
	UINT8	useromeo;
	UINT8	thickframe;
	UINT8	xrollkey;
	UINT8	fscrnbpp;
	UINT8	fscrnmod;
	UINT8	fsrescfg;

#if defined(SUPPORT_SCRN_DIRECT3D)
	UINT8	d3d_imode; // Direct3D interpolation mode
	UINT8	d3d_exclusive; // Direct3D fullscreen exclusive mode
#endif

	UINT8	cSoundDeviceType;
	TCHAR	szSoundDeviceName[MAX_PATH];

#if defined(SUPPORT_VSTi)
	TCHAR	szVSTiFile[MAX_PATH];
#endif	// defined(SUPPORT_VSTi)

	UINT8	emuddraw; // DirectDraw Emulation Only
	UINT8	drawtype; // Screen renderer type (0: DirectDraw, 1: reserved(DirecrDraw), 2: Direct3D)
	UINT8	dragdrop; // Drag and drop support
	UINT8	makelhdd; // Large HDD creation support
	UINT8	syskhook; // Low-level keyboard hook support
	UINT8	rawmouse; // Raw mouse input support
	SINT16	mousemul; // Mouse speed (mul)
	SINT16	mousediv; // Mouse speed (div)
	
	UINT8	scrnmode; // Screen Mode
	UINT8	savescrn; // Save ScreenMode
	
	UINT8	svscrmul; // Save Screen Size Multiplying Value 
	UINT8	scrn_mul; // Screen Size Multiplying Value (8: default)
	
	UINT8	mouse_nc; // Always notify mouse event
	UINT16	cpustabf; // CPU clock stabilizer frame
	UINT8	readonly; // No save changed settings
	UINT8	wheelact; // Mouse wheel action
	UINT8	wheelac2; // Mouse wheel action (+Ctrl or Shift)
	UINT8	tickmode; // Force Set Tick Counter Mode
	//UINT8	usemastervolume; // Use Master Volume
	UINT8	usemidivolume; // Use MIDI Volume
	UINT8	mastervolumemax; // Maxmum Master Volume
	
	UINT8	toolwndhistory; // Number of data of recently opened FD image list in Tool Window
	
#ifdef SUPPORT_WACOM_TABLET
	UINT8	pentabfa; // Pen tablet fixed aspect mode
#endif	// defined(SUPPORT_WACOM_TABLET)
	
#if defined(SUPPORT_MULTITHREAD)
	UINT8	multithread; // Multi Thread Mode
#endif

	UINT8	midiasns; // MIDI Active Sensing?ｿｽ翌?ｿｽ
	UINT32	midiaint; // MIDI Active Sensing?ｿｽ翌?ｿｽﾔ隔?ｿｽi?ｿｽ~?ｿｽ?ｿｽ?ｿｽb?ｿｽj

	UINT8	knjpaste; // ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽv?ｿｽ{?ｿｽ[?ｿｽh?ｿｽ?ｿｽ?ｿｽ?ｿｽe?ｿｽL?ｿｽX?ｿｽg?ｿｽ\?ｿｽ?ｿｽt?ｿｽ?ｿｽ?ｿｽﾌ際の奇ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ茨ｿｽ?ｿｽ?ｿｽ?ｿｽi0=?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ, 1=BASIC, 2=FEP?ｿｽﾈゑｿｽDOS?ｿｽj
	UINT8	scrscfix; // ?ｿｽ?ｿｽﾊ拡?ｿｽ?ｿｽ]?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ補正?ｿｽi0=?ｿｽ?ｿｽ?ｿｽ?ｿｽ, 1=?ｿｽﾊ擾ｿｽ]?ｿｽ?ｿｽ, 2=NVDIA?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽj
	UINT8	dirfdlst; // ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽf?ｿｽB?ｿｽ?ｿｽ?ｿｽN?ｿｽg?ｿｽ?ｿｽ?ｿｽﾉゑｿｽ?ｿｽ?ｿｽFD?ｿｽC?ｿｽ?ｿｽ?ｿｽ[?ｿｽW?ｿｽt?ｿｽ@?ｿｽC?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽX?ｿｽg?ｿｽ?ｿｽ\?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
	UINT8	allports; // ?ｿｽﾝ抵ｿｽﾅシ?ｿｽ?ｿｽ?ｿｽA?ｿｽ?ｿｽ?ｿｽE?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾉ関係?ｿｽﾈゑｿｽ?ｿｽC?ｿｽﾓのポ?ｿｽ[?ｿｽg?ｿｽ?ｿｽI?ｿｽﾗゑｿｽ謔､?ｿｽﾉゑｿｽ?ｿｽ?ｿｽ
	TCHAR	prnfontM[MAX_PATH]; // ?ｿｽ?ｿｽ?ｿｽ?ｿｽt?ｿｽH?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽi?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ）
	TCHAR	prnfontG[MAX_PATH]; // ?ｿｽ?ｿｽ?ｿｽ?ｿｽt?ｿｽH?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽi?ｿｽS?ｿｽV?ｿｽb?ｿｽN?ｿｽﾌ）
	UINT8	prncfgpp; // ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ度?ｿｽﾉ用?ｿｽ?ｿｽ?ｿｽﾝ抵ｿｽ?ｿｽ\?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ

	UINT16	cpumullst[8]; // CPU?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽN?ｿｽ{?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽX?ｿｽg
	UINT16	cpuspdlst[9]; // CPU?ｿｽX?ｿｽs?ｿｽ[?ｿｽh?ｿｽ?ｿｽ?ｿｽX?ｿｽg
} NP2OSCFG;


enum {
	FULLSCREEN_WIDTH	= 640,
	FULLSCREEN_HEIGHT	= 480
};

#ifndef NP2BREAK_FLAGS_DEFINED
enum {
	NP2BREAK_RESUME		= 0x00,
	NP2BREAK_MAIN		= 0x01,
	NP2BREAK_DEBUG		= 0x02
};
#define NP2BREAK_FLAGS_DEFINED
#endif

enum {
	WM_NP2CMD			= (WM_USER + 200)
};

enum {
	NP2CMD_EXIT			= 0,
	NP2CMD_RESET		= 1,
	NP2CMD_EXIT2		= 0x0100,
	NP2CMD_DUMMY		= 0xffff
};

enum {
	MMXFLAG_DISABLE		= 1,
	MMXFLAG_NOTSUPPORT	= 2
};


extern	NP2OSCFG	np2oscfg;
extern	HWND		g_hWndMain;
extern	UINT8		np2break;
extern	BOOL		winui_en;
extern	UINT8		g_scrnmode;
#if !defined(_WIN64)
extern	int			mmxflag;
#endif

extern	OEMCHAR		modulefile[MAX_PATH];
extern	OEMCHAR		fddfolder[MAX_PATH];
extern	OEMCHAR		hddfolder[MAX_PATH];
extern	OEMCHAR		cdfolder[MAX_PATH];
extern	OEMCHAR		bmpfilefolder[MAX_PATH];
extern	OEMCHAR		npcfgfilefolder[MAX_PATH];

void np2active_renewal(void);
void unloadNP2INI();
void loadNP2INI(const OEMCHAR *fname);


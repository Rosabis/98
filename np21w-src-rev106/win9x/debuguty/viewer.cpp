/**
 * @file	viewer.cpp
 * @brief	DebugUty ?ｿｽp?ｿｽr?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽN?ｿｽ?ｿｽ?ｿｽX?ｿｽﾌ難ｿｽ?ｿｽ?ｿｽﾌ抵ｿｽ`?ｿｽ?ｿｽ?ｿｽs?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 */

#include "compiler.h"
#include "resource.h"
#include "np2.h"
#include "viewer.h"
#include "viewitem.h"
#include "viewmenu.h"
#include "viewcmn.h"
#include "d_addr.h"
#include "d_edit.h"
#include "cpucore.h"
#include "mousemng.h"
#include "break.h"

//! 讀懃ｴ｢繝?繝ｼ繧ｿ (IDM_FINDAGAIN 縺ｧ蜀榊茜逕ｨ)
static EDITDATA s_findData = { { 0 }, { 0 }, 0, IDC_EDIT_TYPE_HEX };

static void EnableFindAgain(BOOL bEnable);

//! ?ｿｽC?ｿｽ?ｿｽ?ｿｽX?ｿｽ^?ｿｽ?ｿｽ?ｿｽX
static CDebugUtyView* g_np2view[NP2VIEW_MAX];
static CDebugUtyView* g_np2view_closed[NP2VIEW_MAX];

//! ?ｿｽr?ｿｽ?ｿｽ?ｿｽ[ ?ｿｽN?ｿｽ?ｿｽ?ｿｽX?ｿｽ?ｿｽ
static const TCHAR s_szViewClass[] = TEXT("NP2-ViewWindow");

//! ?ｿｽt?ｿｽH?ｿｽ?ｿｽ?ｿｽg
static const TCHAR s_szViewFont[] = _T("?ｿｽl?ｿｽr ?ｿｽS?ｿｽV?ｿｽb?ｿｽN");

//! ?ｿｽﾅ鯉ｿｽ?ｿｽTick
DWORD CDebugUtyView::sm_dwLastTick;

//! ?ｿｽ`?ｿｽF?ｿｽb?ｿｽN ?ｿｽ}?ｿｽN?ｿｽ?ｿｽ
#define MFCHECK(bChecked) ((bChecked) ? MF_CHECKED : MF_UNCHECKED)

/**
 * ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 * @param[in] hInstance ?ｿｽC?ｿｽ?ｿｽ?ｿｽX?ｿｽ^?ｿｽ?ｿｽ?ｿｽX
 */
void CDebugUtyView::Initialize(HINSTANCE hInstance)
{
	sm_dwLastTick = ::GetTickCount();

	ZeroMemory(g_np2view, sizeof(g_np2view));
	ZeroMemory(g_np2view_closed, sizeof(g_np2view_closed));

	WNDCLASS np2vc;
	np2vc.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
	np2vc.lpfnWndProc = ::DefWindowProc;
	np2vc.cbClsExtra = 0;
	np2vc.cbWndExtra = 0;
	np2vc.hInstance = sm_hInstance;//hInstance; // XXX: ?ｿｽ?ｿｽ?ｿｽ黹奇ｿｽ\?ｿｽ[?ｿｽX?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾆデ?ｿｽo?ｿｽb?ｿｽO?ｿｽ?ｿｽ?ｿｽ[?ｿｽe?ｿｽB?ｿｽ?ｿｽ?ｿｽe?ｿｽB?ｿｽ?ｿｽ?ｿｽ\?ｿｽ?ｿｽ?ｿｽﾅゑｿｽ?ｿｽﾈゑｿｽ?ｿｽﾈゑｿｽﾌゑｿｽ?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ?ｿｽ?ｿｽ np21w ver0.86 rev13
	np2vc.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	np2vc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	np2vc.hbrBackground = static_cast<HBRUSH>(NULL);
	np2vc.lpszMenuName = MAKEINTRESOURCE(IDR_VIEW);
	np2vc.lpszClassName = s_szViewClass;
	::RegisterClass(&np2vc);
}

/**
 * ?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽ?ｿｽ?ｿｽ?成?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
void CDebugUtyView::New()
{
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		CDebugUtyView* lpView = g_np2view[i];
		if (lpView != NULL)
		{
			continue;
		}
		if (g_np2view_closed[i] != NULL)
		{
			delete g_np2view_closed[i];
			g_np2view_closed[i] = NULL;
		}

		CDebugUtyView* view = new CDebugUtyView;
		g_np2view[i] = view;

		if (view->CreateEx(0, s_szViewClass, NULL, WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
		{
			view->ShowWindow(SW_SHOWNORMAL);
			view->UpdateWindow();
		}
		break;
	}
}
/**
 * ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽj?ｿｽ?ｿｽ
 */
void CDebugUtyView::DisposeAllClosedWindow()
{
	for (size_t i = 0; i < _countof(g_np2view_closed); i++)
	{
		if (g_np2view_closed[i] != NULL)
		{
			delete g_np2view_closed[i];
			g_np2view_closed[i] = NULL;
		}
	}
}

/**
 * ?ｿｽ?ｿｽ?ｿｽﾗて閉ゑｿｽ?ｿｽ?ｿｽ
 */
void CDebugUtyView::AllClose()
{
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		CDebugUtyView* lpView = g_np2view[i];
		if (lpView != NULL)
		{
			lpView->DetachDebugView();
			lpView->DestroyWindow();
		}
	}
}

/**
 * ?ｿｽ?ｿｽ?ｿｽﾗて更?ｿｽV
 * @param[in] bForce ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽI?ｿｽﾉ更?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
void CDebugUtyView::AllUpdate(bool bForce)
{
	const DWORD dwNow  = ::GetTickCount();
	if ((!bForce) || ((dwNow - sm_dwLastTick) >= 200))
	{
		sm_dwLastTick = dwNow;

		for (size_t i = 0; i < _countof(g_np2view); i++)
		{
			CDebugUtyView* lpView = g_np2view[i];
			if (lpView != NULL)
			{
				lpView->UpdateView();
			}
		}
	}
}

/**
 * 諷｡蛛ｰ蛛ｺ蜒ｹ蜆丈ｹ募⊆蜆雁Ρ蜆丈ｹ募ｿｬ諛ｺ蛯ｪ蟲乗ｴ蛛｡蛯?
 * @param[in] bRunning 蜒?蜆亥э蜆嶺ｹ募ヴ蜆大?槫￠蟷壼ｳｴ諡槫＄
 */
void CDebugUtyView::AllMenuState(bool bRunning)
{
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		CDebugUtyView* lpView = g_np2view[i];
		if (lpView != NULL)
		{
			viewmenu_debug_toggle(lpView, bRunning ? TRUE : FALSE);
		}
	}
}

/**
 * ?ｿｽR?ｿｽ?ｿｽ?ｿｽX?ｿｽg?ｿｽ?ｿｽ?ｿｽN?ｿｽ^
 */
CDebugUtyView::CDebugUtyView()
	: m_bActive(false)
	, m_nVPos(0)
	, m_nVLines(0)
	, m_nVPage(0)
	, m_nVMultiple(1)
	, m_lpItem(NULL)
	, m_hWndStatus(NULL)
{
}

/**
 * ?ｿｽf?ｿｽX?ｿｽg?ｿｽ?ｿｽ?ｿｽN?ｿｽ^
 */
CDebugUtyView::~CDebugUtyView()
{
	DetachDebugView();
}

/**
 * ?ｿｽf?ｿｽo?ｿｽb?ｿｽO?ｿｽ\?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽ`?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾘり離?ｿｽ?ｿｽ
 */
void CDebugUtyView::DetachDebugView()
{
	if (m_lpItem)
	{
		delete m_lpItem;
		m_lpItem = NULL;
	}

	CloseStatusBar();

	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		if (g_np2view[i] == this)
		{
			g_np2view_closed[i] = g_np2view[i]; // ?ｿｽﾂゑｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽﾆゑｿｽ?ｿｽﾄ登?ｿｽ^
			g_np2view[i] = NULL;
			UpdateActive();
			break;
		}
	}
}

/**
 * ?ｿｽL?ｿｽ?ｿｽ?ｿｽv?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ更?ｿｽV
 */
void CDebugUtyView::UpdateCaption()
{
	int nIndex = -1;
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		if (g_np2view[i] == this)
		{
			nIndex = static_cast<int>(i);
			break;
		}
	}
	LPCTSTR lpMode = (m_lpItem->IsLocked()) ? TEXT("Locked") : TEXT("Realtime");

	TCHAR szTitle[256];
	wsprintf(szTitle, TEXT("%d.%s - NP2 Debug Utility"), nIndex + 1, lpMode);

	SetWindowText(szTitle);
}

/**
 * V ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽﾊ置?ｿｽﾌ設抵ｿｽ
 * @param[in] nPos ?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾊ置
 */
void CDebugUtyView::SetVScrollPos(UINT nPos)
{
	if (m_nVPos != nPos)
	{
		m_nVPos = nPos;
		UpdateVScroll();
		Invalidate();
	}
}

/**
 * V ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽﾌ設抵ｿｽ
 * @param[in] nPos ?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾊ置
 * @param[in] nLines ?ｿｽ?ｿｽ?ｿｽC?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
void CDebugUtyView::SetVScroll(UINT nPos, UINT nLines)
{
	if ((m_nVPos != nPos) || (m_nVLines != nLines))
	{
		m_nVPos = nPos;
		m_nVLines = nLines;
		m_nVMultiple = ((nLines - 1) / 0xFFFF) + 1;
		UpdateVScroll();
		Invalidate();
	}
}

/**
 * V ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾌ更?ｿｽV
 */
void CDebugUtyView::UpdateVScroll()
{
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	si.nMin = 0;
	si.nMax = ((m_nVLines + m_nVMultiple - 1) / m_nVMultiple) - 1;
	si.nPos = m_nVPos / m_nVMultiple;
	si.nPage = m_nVPage / m_nVMultiple;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

/**
 * ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE ?ｿｽv?ｿｽ?ｿｽ?ｿｽV?ｿｽ[?ｿｽW?ｿｽ?ｿｽ
 * @param[in] message ?ｿｽ?ｿｽ?ｿｽb?ｿｽZ?ｿｽ[?ｿｽW
 * @param[in] wParam ?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ^
 * @param[in] lParam ?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ^
 * @return ?ｿｽ?ｿｽ?ｿｽU?ｿｽ?ｿｽ?ｿｽg ?ｿｽR?ｿｽ[?ｿｽh
 */
LRESULT CDebugUtyView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			return OnCreate(reinterpret_cast<LPCREATESTRUCT>(lParam));

		case WM_PAINT:
			OnPaint();
			break;

		case WM_SIZE:
			OnSize(static_cast<UINT>(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_VSCROLL:
			OnVScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
			break;

		case WM_ENTERMENULOOP:
			OnEnterMenuLoop(static_cast<BOOL>(wParam));
			break;

		case WM_ACTIVATE:
			OnActivate(LOWORD(wParam), reinterpret_cast<HWND>(lParam), HIWORD(wParam));
			break;

		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			mousemng_updatemouseon(false);
			break;

		case WM_CLOSE:
			DetachDebugView();
			DestroyWindow();
			break;

		default:
			return CWndProc::WindowProc(message, wParam, lParam);
	}
	return 0;
}

/**
 * ?ｿｽ?ｿｽ?ｿｽ[?ｿｽU?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽﾌ搾ｿｽ?ｿｽﾚゑｿｽI?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾆゑｿｽ?ｿｽﾉ、?ｿｽt?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽN?ｿｽﾉゑｿｽ?ｿｽ?ｿｽﾄ呼び出?ｿｽ?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 * @param[in] wParam ?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ^
 * @param[in] lParam ?ｿｽp?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ^
 * @retval TRUE ?ｿｽA?ｿｽv?ｿｽ?ｿｽ?ｿｽP?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽb?ｿｽZ?ｿｽ[?ｿｽW?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
BOOL CDebugUtyView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	switch (nID)
	{
		case IDM_VIEWWINNEW:
			New();
			break;

		case IDM_VIEWWINCLOSE:
			DetachDebugView();
			DestroyWindow();
			break;

		case IDM_VIEWWINALLCLOSE:
			AllClose();
			break;

		case IDM_VIEWMODEREG:
		case IDM_VIEWMODESEG:
		case IDM_VIEWMODESTK:
		case IDM_VIEWMODE1MB:
		case IDM_VIEWMODEASM:
		case IDM_VIEWMODESND:
			SetMode(nID);
			break;

		case IDM_VIEWMODELOCK:
			if (!m_lpItem->IsLocked())
			{
				m_lpItem->Lock();
			}
			else
			{
				m_lpItem->Unlock();
			}
			UpdateCaption();
			Invalidate();
			break;

#if defined(SUPPORT_NP2BREAK)
		case IDM_DEBUG_RUN:
			np2active_set(1);
			break;

		case IDM_DEBUG_STOP:
			np2active_set(0);
			break;

		case IDM_DEBUG_STEPINTO:
			np2active_step();
			break;

		case IDM_DEBUG_STEPOVER:
			np2active_step_over();
			break;

		case IDM_BREAK_TOGGLE:
			m_lpItem->OnCommand(wParam, lParam);
			break;
#endif

		case IDM_RELOAD:
			m_lpItem->Update();
			Invalidate();
			UpdateStatusBar();
			break;

		case IDM_SETSEG:
			{
				// 繧｢繝峨Ξ繧ｹ蜈･蜉? (d_addr)
				const UINT32 nAddr = m_lpItem->GetAddress();
				const UINT16 nSeg = (m_lpItem->GetID() == IDM_VIEWMODE1MB)
					? (UINT16)((nAddr >> 4) & 0xf000)
					: (UINT16)m_lpItem->GetSegment();
				const UINT16 nOff = (UINT16)(nAddr & 0xffff);
				const UINT32 ret = daddr_input(m_hWnd, nSeg, nOff);
				if (ret != (UINT32)-1)
				{
					m_lpItem->SetAddress(((ret >> 16) << 4) + (ret & 0xffff));
					m_lpItem->Update();
					Invalidate();
					UpdateStatusBar();
				}
			}
			break;

		case IDM_EDIT_MEMORY:
			{
				// 繝｡繝｢繝ｪ邱ｨ髮? (d_edit)
				EDITDATA* ed = dedit_input(m_hWnd, FALSE);
				if ((ed != NULL) && (ed->bytes_len > 0))
				{
					m_lpItem->WriteMemory(m_lpItem->GetAddress(), ed->bytes, ed->bytes_len);
					m_lpItem->Update();
					Invalidate();
					UpdateStatusBar();
				}
			}
			break;

		case IDM_FIND:
			{
				// 讀懃ｴ｢ (d_edit)
				EDITDATA* ed = dedit_input(m_hWnd, TRUE);
				if ((ed != NULL) && (ed->bytes_len > 0))
				{
					s_findData = *ed;
					UINT32 nFound = 0;
					if (m_lpItem->FindMemory(m_lpItem->GetAddress() + 1, ed->bytes, ed->bytes_len, &nFound))
					{
						m_lpItem->SetAddress(nFound);
						m_lpItem->Update();
						SetStatusMessage(_T("Found"));
						EnableFindAgain(TRUE);
					}
					else
					{
						SetStatusMessage(_T("Not found"));
					}
					Invalidate();
					UpdateStatusBar();
				}
			}
			break;

		case IDM_FINDAGAIN:
			if (s_findData.bytes_len > 0)
			{
				UINT32 nFound = 0;
				if (m_lpItem->FindMemory(m_lpItem->GetAddress() + 1, s_findData.bytes, s_findData.bytes_len, &nFound))
				{
					m_lpItem->SetAddress(nFound);
					m_lpItem->Update();
					SetStatusMessage(_T("Found"));
				}
				else
				{
					SetStatusMessage(_T("Not found"));
				}
				Invalidate();
				UpdateStatusBar();
			}
			break;

		default:
			return m_lpItem->OnCommand(wParam, lParam);
	}
	return TRUE;
}

/**
 * ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽﾌ作成?ｿｽ?ｿｽ?ｿｽﾉ、?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾖ撰ｿｽ?ｿｽ?ｿｽ?ｿｽﾄび出?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 * @param[in] lpCreateStruct
 * @retval 0 ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
int CDebugUtyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_lpItem = CDebugUtyItem::New(IDM_VIEWMODEREG, this);
	UpdateCaption();
	OpenStatusBar();
	return 0;
}

/**
 * ?ｿｽt?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽN?ｿｽﾍ、?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽﾌサ?ｿｽC?ｿｽY?ｿｽ?ｿｽ?ｿｽﾏゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾉゑｿｽ?ｿｽﾌ??ｿｽ?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾖ撰ｿｽ?ｿｽ?ｿｽ?ｿｽﾄびまゑｿｽ 
 * @param[in] nType ?ｿｽT?ｿｽC?ｿｽY?ｿｽﾏ更?ｿｽﾌ趣ｿｽ?ｿｽ
 * @param[in] cx ?ｿｽN?ｿｽ?ｿｽ?ｿｽC?ｿｽA?ｿｽ?ｿｽ?ｿｽg?ｿｽﾌ茨ｿｽﾌ新?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 * @param[in] cy ?ｿｽN?ｿｽ?ｿｽ?ｿｽC?ｿｽA?ｿｽ?ｿｽ?ｿｽg?ｿｽﾌ茨ｿｽﾌ新?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ
 */
void CDebugUtyView::OnSize(UINT nType, int cx, int cy)
{
	if (m_hWndStatus != NULL)
	{
		RECT rectStatus;
		GetWindowRect(m_hWndStatus, &rectStatus);
		const int nStatusHeight = rectStatus.bottom - rectStatus.top;
		MoveWindow(m_hWndStatus, 0, cy - nStatusHeight, cx, nStatusHeight, TRUE);
		cy -= nStatusHeight;
	}
	m_nVPage = cy / 16;
	UpdateVScroll();
}

/**
 * ?ｿｽﾄ描?ｿｽ?ｿｽv?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾉ、?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾖ撰ｿｽ?ｿｽ?ｿｽ?ｿｽﾄび出?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 */
void CDebugUtyView::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(&ps);

	RECT rect = ps.rcPaint;
	if (m_hWndStatus != NULL)
	{
		RECT rectStatus;
		GetWindowRect(m_hWndStatus, &rectStatus);
		POINT pt = { 0, 0 };
		ClientToScreen(m_hWnd, &pt);
		const int nStatusTop = rectStatus.top - pt.y;
		if (rect.bottom > nStatusTop)
		{
			rect.bottom = nStatusTop;
		}
	}

	HDC hdcMem = ::CreateCompatibleDC(hDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	hBitmap = static_cast<HBITMAP>(::SelectObject(hdcMem, hBitmap));


	HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 0, 64));
	::FillRect(hdcMem, &rect, hBrush);
	::DeleteObject(hBrush);

	HFONT hFont = ::CreateFont(16, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, s_szViewFont);
	::SetTextColor(hdcMem, 0xffffff);
	::SetBkColor(hdcMem, 0x400000);
	hFont = static_cast<HFONT>(::SelectObject(hdcMem, hFont));

		m_lpItem->OnPaint(hdcMem, rect);

	::DeleteObject(SelectObject(hdcMem, hFont));

	::BitBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdcMem, rect.left, rect.top, SRCCOPY);
	::DeleteObject(::SelectObject(hdcMem, hBitmap));
	::DeleteDC(hdcMem);

	EndPaint(&ps);
}

/**
 * ?ｿｽt?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽN?ｿｽﾍ、?ｿｽ?ｿｽ?ｿｽ[?ｿｽU?ｿｽ[?ｿｽ?ｿｽ?ｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽﾉ撰ｿｽ?ｿｽ?ｿｽ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽo?ｿｽ[?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽN?ｿｽ?ｿｽ?ｿｽ?ｿｽﾆ、?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾖ撰ｿｽ?ｿｽ?ｿｽ?ｿｽﾄび出?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 * @param[in] nSBCode ?ｿｽ?ｿｽ?ｿｽ[?ｿｽU?ｿｽ[?ｿｽﾌ要?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽo?ｿｽ[ ?ｿｽR?ｿｽ[?ｿｽh?ｿｽ?ｿｽ?ｿｽw?ｿｽ閧ｵ?ｿｽﾜゑｿｽ
 * @param[in] nPos ?ｿｽ?ｿｽ?ｿｽﾝのス?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽ{?ｿｽb?ｿｽN?ｿｽX?ｿｽﾌ位置
 * @param[in] hwndScrollBar ?ｿｽX?ｿｽN?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ ?ｿｽo?ｿｽ[ ?ｿｽR?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ
 */
void CDebugUtyView::OnVScroll(UINT nSBCode, UINT nPos, HWND hwndScrollBar)
{
	UINT32 nNewPos = m_nVPos;
	switch (nSBCode)
	{
		case SB_LINEUP:
			if (nNewPos)
			{
				nNewPos--;
			}
			break;

		case SB_LINEDOWN:
			if ((nNewPos + m_nVPage) < m_nVLines)
			{
				nNewPos++;
			}
			break;

		case SB_PAGEUP:
			if (nNewPos > m_nVPage)
			{
				nNewPos -= m_nVPage;
			}
			else
			{
				nNewPos = 0;
			}
			break;

		case SB_PAGEDOWN:
			nNewPos += m_nVPage;
			if (nNewPos > (m_nVLines - m_nVPage))
			{
				nNewPos = m_nVLines - m_nVPage;
			}
			break;

		case SB_THUMBTRACK:
			nNewPos = nPos * m_nVMultiple;
			break;
	}
	SetVScrollPos(nNewPos);
}

/**
 * The framework calls this member function when a menu modal loop has been entered
 * @param[in] bIsTrackPopupMenu Specifies whether the menu involved is a popup menu
 */
void CDebugUtyView::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
{
	HMENU hMenu = GetMenu();
	if (hMenu == NULL)
	{
		return;
	}
	viewmenu_mode(this);

	HMENU hSubMenu = ::GetSubMenu(hMenu, 2);
	if (hSubMenu)
	{
		SetSegmentItem(hSubMenu, IDM_SEGCS, TEXT("CS"), CPU_CS);
		SetSegmentItem(hSubMenu, IDM_SEGDS, TEXT("DS"), CPU_DS);
		SetSegmentItem(hSubMenu, IDM_SEGES, TEXT("ES"), CPU_ES);
		SetSegmentItem(hSubMenu, IDM_SEGSS, TEXT("SS"), CPU_SS);
		DrawMenuBar();
	}
}

/**
 * ?ｿｽt?ｿｽ?ｿｽ?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ[?ｿｽN?ｿｽ?ｿｽ CWnd ?ｿｽﾌオ?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ?ｿｽﾍ費ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽﾆゑｿｽ?ｿｽﾉゑｿｽ?ｿｽﾌ??ｿｽ?ｿｽ?ｿｽ?ｿｽo?ｿｽ[?ｿｽﾖ撰ｿｽ?ｿｽ?ｿｽ?ｿｽﾄび出?ｿｽ?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ
 * @param[in] nState ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾉなゑｿｽ?ｿｽﾄゑｿｽ?ｿｽ驍ｩ?ｿｽ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾉなゑｿｽ?ｿｽﾄゑｿｽ?ｿｽ驍ｩ?ｿｽ?ｿｽ?ｿｽw?ｿｽ閧ｵ?ｿｽﾜゑｿｽ
 * @param[in] hwndOther ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾜゑｿｽ?ｿｽﾍ費ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾉなゑｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE ?ｿｽn?ｿｽ?ｿｽ?ｿｽh?ｿｽ?ｿｽ
 * @param[in] bMinimized ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾜゑｿｽ?ｿｽﾍ費ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu?ｿｽﾉなゑｿｽE?ｿｽB?ｿｽ?ｿｽ?ｿｽh?ｿｽE?ｿｽﾌ最擾ｿｽ?ｿｽ?ｿｽ?ｿｽﾌ擾ｿｽﾔゑｿｽ?ｿｽw?ｿｽ閧ｵ?ｿｽﾜゑｿｽ
 */
void CDebugUtyView::OnActivate(UINT nState, HWND hwndOther, BOOL bMinimized)
{
	m_bActive = (nState != WA_INACTIVE);
	UpdateActive();
}

/**
 * Called by the default OnNcDestroy member function after the window has been destroyed.
 */
void CDebugUtyView::PostNcDestroy()
{
	delete this;
}

/**
 * ?ｿｽ?ｿｽ?ｿｽ[?ｿｽh?ｿｽﾏ更
 * @param[in] nID ?ｿｽ?ｿｽ?ｿｽ[?ｿｽh
 */
void CDebugUtyView::SetMode(UINT nID)
{
	if (m_lpItem->GetID() != nID)
	{
		CDebugUtyItem* lpItem = m_lpItem;
		m_lpItem = CDebugUtyItem::New(nID, this, lpItem);
		delete lpItem;
		Invalidate();
	}
}

/**
 * ?ｿｽr?ｿｽ?ｿｽ?ｿｽ[?ｿｽX?ｿｽV
 */
void CDebugUtyView::UpdateView()
{
	if (m_lpItem->Update())
	{
		Invalidate();
	}
}

/**
 * ?ｿｽ?ｿｽ?ｿｽj?ｿｽ?ｿｽ?ｿｽ[ ?ｿｽA?ｿｽC?ｿｽe?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽX?ｿｽV
 * @param[in] hMenu ?ｿｽ?ｿｽ?ｿｽj?ｿｽ?ｿｽ?ｿｽ[ ?ｿｽn?ｿｽ?ｿｽ?ｿｽh?ｿｽ?ｿｽ
 * @param[in] nId ?ｿｽ?ｿｽ?ｿｽj?ｿｽ?ｿｽ?ｿｽ[ ID
 * @param[in] lpSegment ?ｿｽZ?ｿｽO?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ
 * @param[in] nSegment ?ｿｽZ?ｿｽO?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽg?ｿｽl
 */
void CDebugUtyView::SetSegmentItem(HMENU hMenu, int nId, LPCTSTR lpSegment, UINT nSegment)
{
	TCHAR szString[32];
	wsprintf(szString, TEXT("Seg = &%s [%04x]"), lpSegment, nSegment);
	::ModifyMenu(hMenu, nId, MF_BYCOMMAND | MF_STRING, nId, szString);
}

/**
 * ?ｿｽA?ｿｽN?ｿｽe?ｿｽB?ｿｽu ?ｿｽt?ｿｽ?ｿｽ?ｿｽO?ｿｽ?ｿｽ?ｿｽX?ｿｽV
 */
void CDebugUtyView::UpdateActive()
{
	bool bActive = false;
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		const CDebugUtyView* lpView = g_np2view[i];
		if ((lpView != NULL) && (lpView->m_bActive))
		{
			bActive = true;
		}
	}

	if (bActive)
	{
		np2break |= NP2BREAK_DEBUG;
	}
	else
	{
		np2break &= ~NP2BREAK_DEBUG;
	}
	np2active_renewal();
}

/// Debug utility activity control hooks (break.cpp)
/// -------------------------------------------------
np2break_t	np2break_lasthit_type = NP2BP_NONE;
UINT32		np2break_lasthit_addr = 0;

/*
 * np2active_renewal() (and therefore np2break_ui_toggle()) can be called from
 * the emulation thread when a breakpoint halts the CPU, so the actual window
 * work is deferred to the UI thread, which applies it from
 * framereset_MT_UIThread() via np2break_ui_poll().
 */
static volatile LONG np2break_ui_poll_running = 1;
static volatile LONG np2break_ui_poll_dirty = 0;

void np2break_ui_toggle(BOOL bRunning)
{
	InterlockedExchange(&np2break_ui_poll_running, bRunning ? 1 : 0);
	InterlockedExchange(&np2break_ui_poll_dirty, 1);
}

void np2break_ui_poll(void)
{
	if (InterlockedExchange(&np2break_ui_poll_dirty, 0))
	{
		const bool bRunning = (InterlockedCompareExchange(&np2break_ui_poll_running, 0, 0) != 0);
		CDebugUtyView::AllMenuState(bRunning);
		CDebugUtyView::AllUpdate(true);
	}
}

void np2break_ui_breakpoint(np2break_t type, UINT32 addr)
{
	// Called from the emulation thread when a breakpoint was hit. The debug
	// utility windows refresh themselves from the UI thread
	// (CDebugUtyView::AllUpdate), so all we do here is publishing the hit for
	// the status window to poll.
	np2break_lasthit_type = type;
	np2break_lasthit_addr = addr;
}

/**
 * 繧ｹ繝?繝ｼ繧ｿ繧ｹ繝舌?ｼ繧剃ｽ懈?舌☆繧? (viewstat)
 */
void CDebugUtyView::OpenStatusBar()
{
	if (m_hWndStatus != NULL)
	{
		return;
	}
	m_hWndStatus = CreateWindowEx(0, STATUSCLASSNAME, _T(""),
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP | CCS_BOTTOM,
		0, 0, 0, 0, m_hWnd, NULL, CWndProc::GetResourceHandle(), NULL);
	if (m_hWndStatus == NULL)
	{
		return;
	}
	int widths[] = { 80, 160, -1 };
	SendMessage(m_hWndStatus, SB_SETPARTS, (WPARAM)3, (LPARAM)widths);
	UpdateStatusBar();
}

/**
 * 繧ｹ繝?繝ｼ繧ｿ繧ｹ繝舌?ｼ繧堤ｴ譽?縺吶ｋ
 */
void CDebugUtyView::CloseStatusBar()
{
	if (m_hWndStatus != NULL)
	{
		DestroyWindow(m_hWndStatus);
		m_hWndStatus = NULL;
	}
}

/**
 * 繧ｹ繝?繝ｼ繧ｿ繧ｹ繝舌?ｼ縺ｮ繧ｫ繝ｼ繧ｽ繝ｫ菴咲ｽｮ / CPU 繝｢繝ｼ繝芽｡ｨ遉ｺ繧呈峩譁ｰ縺吶ｋ
 */
void CDebugUtyView::UpdateStatusBar()
{
	if (m_hWndStatus == NULL)
	{
		return;
	}
	TCHAR szAddr[32];
	viewcmn_print_addr(szAddr, m_lpItem->GetSegment(), m_lpItem->GetAddress(),
		(m_lpItem->GetID() != IDM_VIEWMODE1MB) ? TRUE : FALSE);
	SendMessage(m_hWndStatus, SB_SETTEXT, 0, (LPARAM)szAddr);
	SendMessage(m_hWndStatus, SB_SETTEXT, 1,
		(LPARAM)(CPU_STAT_PM ? _T("Protected Mode") : _T("Real Mode")));
}

/**
 * 繧ｹ繝?繝ｼ繧ｿ繧ｹ繝舌?ｼ縺ｮ繝｡繝?繧ｻ繝ｼ繧ｸ谺?繧定ｨｭ螳壹☆繧?
 * @param[in] lpText 繝｡繝?繧ｻ繝ｼ繧ｸ
 */
void CDebugUtyView::SetStatusMessage(LPCTSTR lpText)
{
	if (m_hWndStatus == NULL)
	{
		return;
	}
	SendMessage(m_hWndStatus, SB_SETTEXT, 2, (LPARAM)lpText);
}

/**
 * 陦ｨ遉ｺ繧｢繧､繝?繝縺ｮ ID 繧定ｿ斐☆
 * @return ID
 */
UINT CDebugUtyView::GetItemID() const
{
	return (m_lpItem != NULL) ? m_lpItem->GetID() : 0;
}

/**
 * 陦ｨ遉ｺ繧｢繧､繝?繝縺後Ο繝?繧ｯ縺輔ｌ縺ｦ縺?繧九°
 * @return 繝ｭ繝?繧ｯ縺輔ｌ縺ｦ縺?繧後?ｰ true
 */
bool CDebugUtyView::IsItemLocked() const
{
	return (m_lpItem != NULL) ? (m_lpItem->IsLocked() != false) : false;
}

/**
 * 蜈ｨ縺ｦ縺ｮ繝薙Η繝ｼ縺ｮ Find Again 繝｡繝九Η繝ｼ縺ｮ譛牙柑 / 辟｡蜉ｹ繧貞??繧頑崛縺医ｋ
 * @param[in] bEnable 譛牙柑縺ｫ縺吶ｋ縺?
 */
static void EnableFindAgain(BOOL bEnable)
{
	for (size_t i = 0; i < _countof(g_np2view); i++)
	{
		CDebugUtyView* lpView = g_np2view[i];
		if (lpView != NULL)
		{
			HMENU hMenu = lpView->GetMenu();
			if (hMenu != NULL)
			{
				::EnableMenuItem(hMenu, IDM_FINDAGAIN,
					MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_GRAYED));
			}
		}
	}
}
/// -------------------------------------------------

#include "stdafx.h"
#include "CDPI.h"

typedef HRESULT(WINAPI* func_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS value);
typedef HRESULT(WINAPI* func_GetProcessDpiAwareness)(HANDLE hprocess, PROCESS_DPI_AWARENESS* value);
typedef HRESULT(WINAPI* func_GetDpiForMonitor)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);

static HMODULE						g_hLibrary = NULL;
static func_SetProcessDpiAwareness		g_pSetProcessDpiAwareness = NULL;
static func_GetProcessDpiAwareness		g_pGetProcessDpiAwareness = NULL;
static func_GetDpiForMonitor			g_pGetDpiForMonitor = NULL;

static PROCESS_DPI_AWARENESS g_Awareness = PROCESS_DPI_UNAWARE;

UINT CDPI::g_nScaleFactorSDA = 0;

void CDPI::init()
{
	g_pSetProcessDpiAwareness = NULL;
	g_pGetProcessDpiAwareness = NULL;
	g_pGetDpiForMonitor = NULL;
	g_hLibrary = NULL;

	g_hLibrary = ::LoadLibrary(_T("Shcore.dll"));
	if (g_hLibrary)
	{
		g_pSetProcessDpiAwareness = (func_SetProcessDpiAwareness)::GetProcAddress(g_hLibrary, "SetProcessDpiAwareness");
		g_pGetProcessDpiAwareness = (func_GetProcessDpiAwareness)::GetProcAddress(g_hLibrary, "GetProcessDpiAwareness");
		g_pGetDpiForMonitor = (func_GetDpiForMonitor)::GetProcAddress(g_hLibrary, "GetDpiForMonitor");
	}
}

void CDPI::destroy()
{
	if (g_hLibrary)
	{
		::FreeLibrary(g_hLibrary);

		g_pSetProcessDpiAwareness = NULL;
		g_pGetProcessDpiAwareness = NULL;
		g_pGetDpiForMonitor = NULL;
		g_hLibrary = NULL;
	}
}

// Rectangle helper functions
int RectHeight(RECT Rect)
{
	return Rect.bottom - Rect.top;
}

int RectWidth(RECT Rect)
{
	return Rect.right - Rect.left;
}


CDPI::CDPI()
{
	m_nScaleFactor = 0;
}

UINT CDPI::getGlobalScaleFactor()
{
	return g_nScaleFactorSDA == 0 ? 100 : g_nScaleFactorSDA;
}

BOOL CDPI::GlobalIsStandardScale()
{
	return g_nScaleFactorSDA == 0 || g_nScaleFactorSDA == 100;
}

int  CDPI::Scale(int x) const
{
	// DPI Unaware:  Return the input value with no scaling.
	// These apps are always virtualized to 96 DPI and scaled by the system for the DPI of the monitor where shown.
	if (g_Awareness == PROCESS_DPI_UNAWARE)
	{
		return x;
	}

	// System DPI Aware:  Return the input value scaled by the factor determined by the system DPI when the app was launched.
	// These apps render themselves according to the DPI of the display where they are launched, and they expect that scaling
	// to remain constant for all displays on the system.
	// These apps are scaled up or down when moved to a display with a different DPI from the system DPI.
	if (g_Awareness == PROCESS_SYSTEM_DPI_AWARE)
	{
		return MulDiv(x, g_nScaleFactorSDA, 100);
	}

	// Per-Monitor DPI Aware:  Return the input value scaled by the factor for the display which contains most of the window.
	// These apps render themselves for any DPI, and re-render when the DPI changes (as indicated by the WM_DPICHANGED window message).
	return MulDiv(x, m_nScaleFactor, 100);
}

UINT CDPI::GetScale() const
{
	if (g_Awareness == PROCESS_DPI_UNAWARE)
	{
		return 100;
	}

	if (g_Awareness == PROCESS_SYSTEM_DPI_AWARE)
	{
		return g_nScaleFactorSDA;
	}

	return m_nScaleFactor;
}

void CDPI::SetScale(UINT iDPI)
{
	m_nScaleFactor = MulDiv(iDPI, 100, 96);
	int remainder = m_nScaleFactor % 25;
	int trade = m_nScaleFactor / 25;
	if (remainder != 0)
	{
		if (remainder > 0 && remainder <= 12)
			m_nScaleFactor = 25 * trade;
		else if (remainder > 12 && remainder < 25)
			m_nScaleFactor = 25 * (trade + 1);
	}
	if (g_nScaleFactorSDA == 0)
	{
		g_nScaleFactorSDA = m_nScaleFactor;  // Save the first scale factor, which is all that SDA apps know about
	}
	return;
}

PROCESS_DPI_AWARENESS CDPI::GetAwareness()
{
	if (g_pGetProcessDpiAwareness)
	{
		HANDLE hProcess;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
		g_pGetProcessDpiAwareness(hProcess, &g_Awareness);
	}
	//else
	//	AfxMessageBox(_T("GetProcessAwareneww null"), MB_ICONERROR);
	return g_Awareness;
}

void CDPI::SetAwareness(PROCESS_DPI_AWARENESS awareness)
{
	if (g_pSetProcessDpiAwareness)
	{
		HRESULT hr = E_FAIL;
		hr = g_pSetProcessDpiAwareness(awareness);
		if (hr == S_OK)
		{
			g_Awareness = awareness;
		}
	}

	return;
}

// Scale rectangle from raw pixels to relative pixels.
void CDPI::ScaleRect(RECT* pRect) const
{
	pRect->left = Scale(pRect->left);
	pRect->right = Scale(pRect->right);
	pRect->top = Scale(pRect->top);
	pRect->bottom = Scale(pRect->bottom);
}

void CDPI::ScaleSize(SIZE* pSize) const
{
	pSize->cx = Scale(pSize->cx);
	pSize->cy = Scale(pSize->cy);
}

// Scale Point from raw pixels to relative pixels.
void CDPI::ScalePoint(POINT* pPoint) const
{
	pPoint->x = Scale(pPoint->x);
	pPoint->y = Scale(pPoint->y);
}

UINT CDPI::GetZoom4Plugin()
{
	init();

	UINT     dpix = 96, dpiy = 0;

	if (!g_pGetDpiForMonitor)
		return dpix;

	HMONITOR hMonitor;
	POINT    pt;
	HRESULT  hr = E_FAIL;

	// Get the DPI for the main monitor, and set the scaling factor
	pt.x = 0;
	pt.y = 0;
	hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	hr = g_pGetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy);

	destroy();

	return dpix;
}

void CDPI::InitAwarenessByPointInMonitor(LONG x, LONG y)
{
	if (!g_pGetDpiForMonitor)
		return;

	HMONITOR hMonitor;
	POINT    pt;
	UINT     dpix = 0, dpiy = 0;
	HRESULT  hr = E_FAIL;

	// Get the DPI for the main monitor, and set the scaling factor
	pt.x = x;
	pt.y = y;
	hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	hr = g_pGetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy);

	if (hr == S_OK)
		SetScale(dpix);
}

HWND CDPI::dpiCreateWindow(_In_opt_ LPCTSTR   lpClassName,
	_In_opt_ LPCTSTR   lpWindowName,
	_In_     DWORD     dwStyle,
	_In_     int       x,
	_In_     int       y,
	_In_     int       nWidth,
	_In_     int       nHeight,
	_In_opt_ HWND      hWndParent,
	_In_opt_ HMENU     hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID    lpParam
)
{
	x = Scale(x);
	y = Scale(y);
	nWidth = Scale(nWidth);
	nHeight = Scale(nHeight);
	return ::CreateWindow(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HFONT  CDPI::dpiCreateFontIndirect(_In_ LOGFONT* lplf)
{
	if (lplf)
		lplf->lfHeight = -Scale(-lplf->lfHeight);
	return ::CreateFontIndirect(lplf);
}

BOOL  CDPI::dpiSetWindowPos(_In_ HWND hWnd,
	_In_opt_ HWND hWndInsertAfter,
	_In_ int X,
	_In_ int Y,
	_In_ int cx,
	_In_ int cy,
	_In_ UINT uFlags)
{
	X = Scale(X);
	Y = Scale(Y);
	cx = Scale(cx);
	cy = Scale(cy);
	return ::SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

CDPI& CDPI::get()
{
	static CDPI g_Instane;

	return g_Instane;
}

DOUBLE CDPI::Scale(DOUBLE x)
{
	double dpi = 1.0;
	UINT nZoom = get().GetZoom4Plugin();
	dpi = nZoom / 96.0;

	return x * dpi;
}
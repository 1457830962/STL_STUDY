#pragma once
typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

class CDPI
{
public:
	CDPI();

	void init();
	void destroy();

	static CDPI& get();
	static DOUBLE Scale(DOUBLE x);

	UINT GetZoom4Plugin();

	HWND dpiCreateWindow(_In_opt_ LPCTSTR   lpClassName,
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
	);
	HFONT  dpiCreateFontIndirect(_In_ LOGFONT* lplf);

	BOOL   dpiSetWindowPos(_In_ HWND hWnd,
		_In_opt_ HWND hWndInsertAfter,
		_In_ int X,
		_In_ int Y,
		_In_ int cx,
		_In_ int cy,
		_In_ UINT uFlags);

	int  Scale(int x) const;
	UINT GetScale() const;
	void SetScale(UINT iDPI);

	// Scale rectangle from raw pixels to relative pixels.
	void ScaleRect(RECT* pRect) const;

	void ScaleSize(SIZE* pSize) const;

	// Scale Point from raw pixels to relative pixels.
	void ScalePoint(POINT* pPoint) const;


	POINT& Scale(POINT& p) const {
		ScalePoint(&p); return p;
	}
	RECT& Scale(RECT& p) const {
		ScaleRect(&p); return p;
	}
	SIZE& Scale(SIZE& p) const {
		ScaleSize(&p); return p;
	}

	void InitAwarenessByPointInMonitor(LONG x, LONG y);

	static PROCESS_DPI_AWARENESS GetAwareness();
	static void SetAwareness(PROCESS_DPI_AWARENESS awareness);
	static UINT	getGlobalScaleFactor();
	static BOOL GlobalIsStandardScale();


private:
	UINT m_nScaleFactor;
	static UINT g_nScaleFactorSDA;
	static BOOL g_boUseGDIPlusWhileScale;
};
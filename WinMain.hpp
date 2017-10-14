#pragma once
namespace ark {
	class WinClass {
		static const auto CreateWndClassEx() {
			WNDCLASSEX wex;
			ZeroMemory(&wex, sizeof(wex));
			wex.cbSize = sizeof(WNDCLASSEX);
			wex.style = CS_HREDRAW | CS_VREDRAW;
			wex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			wex.lpszMenuName = nullptr;
			wex.lpszClassName = _T("ArkLib");
			return wex;
		}
		struct WinMainArguments {
			HINSTANCE hInstance;
			HINSTANCE hPrevInstance;
			LPCTSTR lpCmdLine;
			int nCmdShow;
		};
		WNDCLASSEX WndClass_Ex;
		WinMainArguments WinMain_Arg;
		static WinClass* ptr;
		static HWND hWnd;
		static bool endflag;
		const auto trans(const int x, const int y, const float xratio, const float yratio) {
			std::pair<int, int> pair;
			if (x / xratio == y) {
				pair.first = x;
				pair.second = y;
			}
			else if (x / xratio*yratio < y) {
				pair.first = x;
				pair.second = x / xratio*yratio;
			}
			else {
				pair.first = y / yratio * xratio;
				pair.second = y;
			}
			return pair;
		}
	public:
		WinClass() :WinClass(CreateWndClassEx()) {}
		WinClass(WNDCLASSEX wex) {
			HINSTANCE hinst = GetModuleHandle(nullptr);
			ptr = this;
			endflag = false;
			WinMain_Arg.hInstance = hinst;
			WinMain_Arg.hPrevInstance = nullptr;
			WinMain_Arg.lpCmdLine = GetCommandLine();
			WinMain_Arg.nCmdShow = SW_SHOWDEFAULT;
			WndClass_Ex = wex;
			WndClass_Ex.hInstance =hinst;
			WndClass_Ex.lpfnWndProc = CallProc;
			RegisterClassEx(&WndClass_Ex);
			hWnd = CreateWindowEx(0, WndClass_Ex.lpszClassName, _T("ArkLib"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr, hinst, static_cast<LPVOID>(this));
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);
			SetWindowLong(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));

		}
		static LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			auto ptr_ = reinterpret_cast<WinClass*>(GetWindowLong(WinClass::ptr->hWnd, GWLP_USERDATA));
			return ptr_ ? WinClass::ptr->MainProc(hwnd, msg, wParam, lParam) : DefWindowProc(hwnd, msg, wParam, lParam);
		}
		LRESULT MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			PAINTSTRUCT ps;
			switch (msg) {
			case WM_PAINT:
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
				return 0;
			case WM_ERASEBKGND:
			{
				return FALSE;
			}
			case WM_DESTROY:
				PostQuitMessage(0);
				endflag = true;
				return 0;
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		}
		static const auto GethWnd() { return hWnd; }
		const RECT GetSize() {
			RECT rect;
			GetClientRect(hWnd, &rect);
			return rect;
		}
#define SETFUNC(name,index) auto Set##name(const LONG lp){SetWindowLongPtr(hWnd, index, lp);return *this;}
#define ADDFUNC(name,index) auto Add##name(const LONG lp){const LONG _lp=GetWindowLongPtr(hWnd,index);SetWindowLongPtr(hWnd, index, _lp | lp);return *this;}
		SETFUNC(Style, GWL_STYLE)
		SETFUNC(EXStyle,GWL_EXSTYLE)
		ADDFUNC(Style,GWL_STYLE)
		ADDFUNC(EXStyle,GWL_EXSTYLE)
#undef SETFUNC
#undef ADDFUNC

#define SETFUNC(name,index) static auto& Set##name(const LONG lp){SetClassLongPtr(hWnd, index, lp);return *ptr;}
#define ADDFUNC(name,index) static auto& Add##name(const LONG lp){const LONG _lp=GetClassLongPtr(hWnd,index);SetClassLongPtr(hWnd, index, _lp | lp);return *ptr;}
		SETFUNC(Cursor,GCLP_HCURSOR)
		SETFUNC(Icon,GCLP_HICON)
		SETFUNC(IconSM,GCLP_HICONSM)
		ADDFUNC(Cursor, GCLP_HCURSOR)
		ADDFUNC(Icon, GCLP_HICON)
		ADDFUNC(IconSM, GCLP_HICONSM)
#undef SETFUNC
#undef ADDFUNC
		static auto& SetTitle(LPCTSTR lp) {
			SetWindowText(hWnd, lp);
			return *ptr;
		}
		static auto& SetSize(int hx, int hy) {
			RECT rc;
			SetWindowPos(hWnd, nullptr, 0, 0, hx, hy, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
			GetClientRect(hWnd, &rc);
			const int fx = hx - (rc.right - rc.left);
			const int fy = hy - (rc.bottom - rc.top);
			SetWindowPos(hWnd, nullptr, 0, 0, hx + fx, hy + fy, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
			return *ptr;
		}

			static const bool SetWindowElements(int index, LONG lp) {
			if (SetWindowLongPtr(hWnd, index, lp))return true;
			return false;
		}

		static const bool SetWndClassExElements(int index, LONG lp) {
			if (SetClassLongPtr(hWnd, index, lp))return true;
			return false;
		}
		static const bool SetWindowPosition(HWND z, int x, int y, int cx, int cy, UINT uf = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW) {
			if (SetWindowPos(hWnd, z, x, y, cx, cy, uf))return true;
			return false;
		}
		static auto& SetPosition(int x, int y) {
			SetWindowPos(hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
			return *ptr;
		}
		//[WIP]
		/*bool Set_Transparent(const COLORREF color, const int alpha, const DWORD dw) {
			BLENDFUNCTION bf{};
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.SourceConstantAlpha = 105;
			RECT rc;
			GetClientRect(hWnd, &rc);
			SIZE size = { 800,600 };
			UPDATELAYEREDWINDOWINFO info{};
			info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
			info.psize = &size;
			info.crKey = RGB(0, 0, 0);
			info.pblend = &bf;
			info.dwFlags = ULW_ALPHA;
			info.hdcSrc = GetDC(hWnd);
			UpdateLayeredWindowIndirect(hWnd, &info);
			return 0;

		}*/
		
		static auto& LockAspectRatio(const float xratio, const float yratio) {
			RECT rc;
			GetClientRect(Ark::WinClass::GethWnd(), &rc);
			auto pair = ptr->trans(rc.right, rc.bottom, xratio, yratio);
			if (!(pair.first == rc.right && pair.second == rc.bottom)) {
				Ark::WinClass::SetSize(pair.first, pair.second);
			}
			return *ptr;
		}
		static const bool EndFlag() {
			MSG msg;
			if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
				if (msg.message != WM_QUIT) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else Exit();
			}
			return endflag;
		}
		static const bool Exit() { endflag = true; return endflag; }
	};
	WinClass* WinClass::ptr = nullptr;
	HWND WinClass::hWnd = nullptr;
	bool WinClass::endflag = false;
}

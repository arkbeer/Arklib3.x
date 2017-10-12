#pragma once
#include<utility>
namespace ark {
	struct Mouse {
		static auto GetScreenPosition() {
			POINT pt;
			GetCursorPos(&pt);
			return std::make_pair(pt.x, pt.y);
		}
		static auto GetClientPosition(HWND hWnd) {
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			return std::make_pair(pt.x, pt.y);
		}
	};
}

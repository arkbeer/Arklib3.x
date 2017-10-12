#pragma once
#include<utility>
namespace ark {
	namespace Mouse {
		auto GetScreenPosition() {
			POINT pt;
			GetCursorPos(&pt);
			return std::make_pair(pt.x, pt.y);
		}
		auto GetClientPosition(HWND hWnd) {
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			return std::make_pair(pt.x, pt.y);
		}
	}
}

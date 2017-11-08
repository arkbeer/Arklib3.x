#include<array>
#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include"Shape.hpp"
#include"D2D1_1.hpp"
#include"WinMain.hpp"
#include"Tstring.hpp"
#include"Fps.hpp"
#include"Key.hpp"
#include"Random.hpp"
ark::WinClass wc;
ark::D2D1_1 d;
struct Chip {
	enum class Type {
		Blank
	};
	enum class Cond {
		None
	};
	Type type;
	Cond cond;
	int x, y;
};
template<int X,int Y>
auto make_blank_map() {
	std::array<std::array<Chip, X>, Y> map;
	for (int i=0; i < Y; ++i) {
		for (int j=0; j < X; ++j) {
			map[i][j].x = j;
			map[i][j].y = i;
		}
	}
	return map;
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("Realm"));
	d.Initialize();
	auto icon = d.SetBitmap(L"icon.png");
	auto map = make_blank_map<16,9>();
	while (!wc.EndFlag()) {
		d.BeginDraw();
		d.DrawClear(RGB(0,0,0));
		wc.LockAspectRatio(16, 9);
		RECT rc = wc.GetSize();
		d.DrawBitmap(icon,0,0,200,200,0,0,200,200,1.0f);
		for (auto& v1:map) {
			for (auto& v2:v1) {
				d.DrawRectangle(v2.x*rc.right/(map.at(0).size()),v2.y*rc.bottom / (map.size()), (v2.x+1)*rc.right / (map.at(0).size()), (v2.y+1)*rc.bottom / (map.size()), RGB(255,0,0),1.0f,1);
			}
		}
		d.EndDraw();
	}
	return 0;
}

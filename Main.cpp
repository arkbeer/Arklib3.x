#include<Windows.h>
#include<tchar.h>
#include<iostream>
#include"WinMain.hpp"
#include"Shape.hpp"
#include"D2D1_1.hpp"
ark::WinClass wc;
ark::D2D1_1 d;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	d.Initialize();
	auto bmp = d.SetBitmap(L"test.jpg");
	while (true) {
		d.BeginDraw();
		d.DrawBitmap(bmp,0,0,100,100,0,0,100,100,1.0f);
		d.EndDraw();
	}
	return 0;
}
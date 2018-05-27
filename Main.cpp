#include<array>
#include<vector>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include"WinMain.hpp"
#include"Mouse.hpp"
#include"Fps.hpp"
#include"Key.hpp"
#include"Random.hpp"
#include"D3D11_1.hpp"
#include"WIC.hpp"
#include"Matrix.hpp"
#include"Tstring.hpp"
ark::Fps fps;
ark::Random rnd;
ark::Key key;
int Main();
bool flag = true; int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	return Main();
}
int Main() {
	ark::WinClass wc;
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("Direct3D Example")).SetSize(400, 400);
	ark::D3D11_1 d;
	ark::WIC w;
	auto image = w.loadimage(_T("texture.bmp"));
	auto m=ark::Mouse::GetClientPosition(GetActiveWindow());
	ark::D3D11_1::Texture tex;
	while (wc.Loop()) {
		auto s = std::chrono::steady_clock::now();
		wc.LockAspectRatio(1, 1);
		d.BeginDraw();
		d.SetTexture(tex, image);
		d.DrawClear();
		if (key.KeyDown('W', true))d.SetView(d.GetView()*DirectX::XMMatrixTranslation(0, 0, -0.1f));
		if (key.KeyDown('S', true))d.SetView(d.GetView()*DirectX::XMMatrixTranslation(0, 0, 0.1f));
		if (key.KeyDown('D', true))d.SetView(d.GetView()*DirectX::XMMatrixTranslation(-0.1f, 0, 0));
		if (key.KeyDown('A', true))d.SetView(d.GetView()*DirectX::XMMatrixTranslation(0.1f, 0, 0));
		if (key.KeyDown(VK_LBUTTON, true)) {
			auto m2=ark::Mouse::GetClientPosition(GetActiveWindow());
			m2 = ark::Mouse::GetClientPosition(GetActiveWindow());
			if (!(m.first == m2.first && m.second == m2.second)) {
				m2.first = m2.first - m.first;
				m2.second = m2.second - m.second;
				d.SetView(ark::Matrix(d.GetView()).RotationY(-1.0f*m2.first / wc.GetSize().right*3.141592).RotationX(-1.0f*m2.second / wc.GetSize().bottom*3.141592).GetMatrix());
				m = ark::Mouse::GetClientPosition(GetActiveWindow());
			}
		}
		else m = ark::Mouse::GetClientPosition(GetActiveWindow());

		if (key.KeyDown('1', false)) {
			DirectX::XMVECTOR eye{ 0.0f,2.0f,0.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,0.0f,1.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		if (key.KeyDown('2', false)) {
			DirectX::XMVECTOR eye{ 0.0f,0.0f,-2.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,1.0f,0.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		if (key.KeyDown('3', false)) {
			DirectX::XMVECTOR eye{ 0.0f,-2.0f,0.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,0.0f,1.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		if (key.KeyDown('4', false)) {
			DirectX::XMVECTOR eye{ 0.0f,0.0f,2.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,1.0f,0.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		if (key.KeyDown('5', false)) {
			DirectX::XMVECTOR eye{ -2.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,1.0f,0.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		if (key.KeyDown('6', false)) {
			DirectX::XMVECTOR eye{ 2.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR at{ 0.0f,0.0f,0.0f,0.0f };
			DirectX::XMVECTOR up{ 0.0f,1.0f,0.0f,0.0f };
			auto View = DirectX::XMMatrixLookAtLH(eye, at, up);
			d.SetView(View);
		}
		static float angle = 0.0f;
		d.DrawCube(ark::Matrix().Identity().Scaling(0.8,0.8,0.8).GetMatrix(), tex);
		d.DrawCube(ark::Matrix().Identity().Scaling(0.3, 0.3, 0.3).RotationZ(angle).Translation(-4, 0, 0).RotationY(angle * 2).RotationX(angle * 3).GetMatrix(), tex);
		angle += 0.01f;

		d.EndDraw();
		fps.Count();
		ark::TstringStream tstr;
		tstr << fps.perDuration<double>(s) << "\n";
		OutputDebugString(tstr.str().c_str());
	}
	return 0;
}
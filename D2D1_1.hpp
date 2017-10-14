#pragma once
#include<wrl.h>
#include<atlbase.h>
#incude<tchar.h>
/*Include(WIC)*/
#include<wincodec.h>
#include<wincodecsdk.h>
#pragma comment(lib,"WindowsCodecs.lib")
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d3d11_1.h>
#include <d2d1effects.h>
#include <d2d1effecthelpers.h>
#include <dwrite_1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"dwrite.lib")
namespace ark {
	class D2D1_1 {
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> D2DContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> DXGISwapChain;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> D2DSCBrush;
		Microsoft::WRL::ComPtr<IWICImagingFactory2> IWICIFactory;
		Microsoft::WRL::ComPtr<IDWriteFactory1> IDWFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> IDWFormat;
		Microsoft::WRL::ComPtr<ID2D1Factory1> D2DFactory;
		RECT size;
		HWND hwnd;
		const bool CompareRect(const RECT& f, const RECT& s) {
			if (f.bottom == s.bottom && f.left == s.left && f.right == s.right && f.top == s.top)return true;
			else return false;
		}

		bool CreateD2DResource(HWND hwnd) {
			RECT rect;
			GetClientRect(hwnd, &rect);
			if (!CompareRect(size, rect)) {
				D2DContext.ReleaseAndGetAddressOf();
				D2DSCBrush.ReleaseAndGetAddressOf();
				DXGISwapChain.ReleaseAndGetAddressOf();
			}
			if (!D2DContext || !DXGISwapChain) {
				D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&D2DFactory));
				Microsoft::WRL::ComPtr<ID2D1Device> D2DDevice;
				Microsoft::WRL::ComPtr<ID2D1Bitmap1> D2DBitmap;
				Microsoft::WRL::ComPtr<ID3D11Device> D3D11Device;
				Microsoft::WRL::ComPtr<IDXGIDevice1> DXGIDevice;
				Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
				Microsoft::WRL::ComPtr<IDXGIFactory2> DXGIFactory;
				Microsoft::WRL::ComPtr<IDXGISurface> DXGISurface;

				GetClientRect(hwnd, &size);

				D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED, nullptr, 0, D3D11_SDK_VERSION, &D3D11Device, nullptr, nullptr);
				D3D11Device.As(&DXGIDevice);
				DXGIDevice->GetAdapter(&DXGIAdapter);
				DXGIAdapter->GetParent(IID_PPV_ARGS(&DXGIFactory));
				DXGIDevice->SetMaximumFrameLatency(1);

				DXGI_SWAP_CHAIN_DESC1 desc = { 0 };
				desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				desc.SampleDesc.Count = 1;
				desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				desc.BufferCount = 2;
				desc.Scaling = DXGI_SCALING_NONE;
				desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
				DXGIFactory->CreateSwapChainForHwnd(D3D11Device.Get(), hwnd, &desc, nullptr, nullptr, &DXGISwapChain);
				DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&DXGISurface));
				D2DFactory->CreateDevice(DXGIDevice.Get(), &D2DDevice);
				D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &D2DContext);
				const auto bp = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
				D2DContext->CreateBitmapFromDxgiSurface(DXGISurface.Get(), &bp, &D2DBitmap);
				D2DContext->SetTarget(D2DBitmap.Get());
				D2DContext->CreateSolidColorBrush(D2D1::ColorF(RGB(0, 0, 0), 1.0f), &D2DSCBrush);
			}
			return true;
		}
		const int SetRGB(COLORREF& color) {
			int i = color % 0x100;
			color /= 0x100;
			return i;
		}
		void ConvertRGB(int& r, int& g, int& b, COLORREF& color) {
			r = SetRGB(color);
			g = SetRGB(color);
			b = SetRGB(color);
		}


	public:
		using Bitmap_type = Microsoft::WRL::ComPtr<ID2D1Bitmap1>;
		D2D1_1() {
			CoInitialize(nullptr);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory1), &IDWFactory);
			CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, &IWICIFactory);
			hwnd = nullptr;
		}
		~D2D1_1() {
			IWICIFactory.ReleaseAndGetAddressOf();
			CoUninitialize();
		}
		void BeginDraw() {
			if (hwnd == nullptr) hwnd = GetActiveWindow();
			if (CreateD2DResource(hwnd))D2DContext->BeginDraw();
		}
		void EndDraw() {

			if (D2DContext->EndDraw() == D2DERR_RECREATE_TARGET) {
				D2DContext.ReleaseAndGetAddressOf();
				D2DSCBrush.ReleaseAndGetAddressOf();
				DXGISwapChain.ReleaseAndGetAddressOf();
			}
			else {
				DXGI_PRESENT_PARAMETERS parameters = {};
				DXGISwapChain->Present1(1, 0, &parameters);
			}
		}
		void Initialize() {
			if (hwnd == nullptr) hwnd = GetActiveWindow();
			CreateD2DResource(hwnd);
		}
		void DrawClear(COLORREF color) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DContext->Clear(D2D1::ColorF(RGB(b, g, r)));
		}
		void DrawString(const float x, const float y, LPCWSTR lp, COLORREF color, const float f, const float size) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			IDWFactory->CreateTextFormat(L"メイリオ", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL, size, L"ja-jp", &IDWFormat);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->DrawText(lp, lstrlenW(lp), IDWFormat.Get(), D2D1::RectF(x, y, x + lstrlenW(lp)*size, y + size), D2DSCBrush.Get());
			IDWFormat.ReleaseAndGetAddressOf();
		}

		void DrawLine(const float x1, const float y1, const float x2, const float y2, COLORREF color, const float f, const float size) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), D2DSCBrush.Get(), size);
		}
		void DrawRectangle(const float x1, const float y1, const float x2, const float y2, COLORREF color, const float f) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->FillRectangle(D2D1::RectF(x1, y1, x2, y2), D2DSCBrush.Get());
		}
		void DrawRectangle(const float x1, const float y1, const float x2, const float y2, COLORREF color, const float f, const float size) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->DrawRectangle(D2D1::RectF(x1, y1, x2, y2), D2DSCBrush.Get(), size);
		}void DrawRoundRectangle(const float x1, const float y1, const float x2, const float y2, const float rx, const float ry, COLORREF color, const float f) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x1, y1, x2, y2), rx, ry), D2DSCBrush.Get());
		}
		void DrawRoundRectangle(const float x1, const float y1, const float x2, const float y2, const float rx, const float ry, COLORREF color, const float f, const float size) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(x1, y1, x2, y2), rx, ry), D2DSCBrush.Get(), size);
		}
		void DrawEllipse(const float x, const float y, const float rx, const float ry, COLORREF color, const float f) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), D2DSCBrush.Get());
		}
		void DrawEllipse(const float x, const float y, const float rx, const float ry, COLORREF color, const float f, const float size) {
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			D2DContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry), D2DSCBrush.Get(), size);
		}
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> SetBitmap(LPCWSTR image) {
			Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
			if (IWICIFactory && !bitmap) {
				Microsoft::WRL::ComPtr<IWICBitmapDecoder> dec;
				IWICIFactory->CreateDecoderFromFilename(image, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &dec);
				Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
				dec->GetFrame(0, &frame);
				Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
				IWICIFactory->CreateFormatConverter(&converter);
				converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
				if (D2DContext)D2DContext->CreateBitmapFromWicBitmap(converter.Get(), NULL, &bitmap);
			}
			return bitmap;
		}
		void DrawBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1>& bitmap, const float x1, const float y1, const float x2, const float y2, const float bx1, const float by1, const float bx2, const float by2, const float f) {
			D2DContext->DrawBitmap(bitmap.Get(), D2D1::RectF(x1, y1, x2, y2), f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(bx1, by1, bx2, by2));
		}
		void Transform(D2D1::Matrix3x2F& matrix) {
			D2DContext->SetTransform(matrix);
		}
		D2D1::Matrix3x2F Rotation(const float angle, const float x, const float y) {
			return D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(x, y));
		}
		D2D1::Matrix3x2F Scale(const float scalex, const float scaley, const float x, const float y) {
			return D2D1::Matrix3x2F::Scale(D2D1::SizeF(scalex, scaley), D2D1::Point2F(x, y));
		}
		D2D1::Matrix3x2F Translation(const float x, const float y) {
			return D2D1::Matrix3x2F::Translation(D2D1::SizeF(x, y));
		}
		D2D1::Matrix3x2F Skew(const float anglex, const float angley, const float x, const float y) {
			return D2D1::Matrix3x2F::Skew(anglex, angley, D2D1::Point2F(x, y));
		}
	public:
		template<int N>
		void DrawShape(const Shape<N>& sp, COLORREF color, const double f, const double size = 0) {
			Microsoft::WRL::ComPtr<ID2D1PathGeometry1> PathGeometry;
			Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
			D2DFactory->CreatePathGeometry(&PathGeometry);
			if (SUCCEEDED(PathGeometry->Open(&sink))) {
				sink->BeginFigure(D2D1::Point2F(sp.Vertex[0].x, sp.Vertex[0].y), D2D1_FIGURE_BEGIN_FILLED);
				for (const auto& v : sp.Vertex) {
					if (v != sp.Vertex[0]) {
						sink->AddLine(D2D1::Point2F(v.x, v.y));
					}
					v != sp.Vertex[0];
				}
				sink->EndFigure(D2D1_FIGURE_END_CLOSED);
				sink->Close();
			}
			int r, g, b;
			ConvertRGB(r, g, b, color);
			D2DSCBrush->SetColor(D2D1::ColorF(RGB(b, g, r), f));
			if (size != 0) {
				D2DContext->DrawGeometry(PathGeometry.Get(), D2DSCBrush.Get(), size);
			}
			else D2DContext->FillGeometry(PathGeometry.Get(), D2DSCBrush.Get(), NULL);
		}
		template<>
		void DrawShape(const Line& sp, COLORREF color, const double f, const double size) {
			DrawLine(sp.Vertex[0].x, sp.Vertex[0].y, sp.Vertex[1].x, sp.Vertex[1].y, color, f, size);
		}
		void Draw_Shape(const Rectangle& rc, COLORREF color, const double f, const double size) {
			if (size == 0) {
				DrawRectangle(rc.Vertex[0].x, rc.Vertex[0].y, rc.Vertex[2].x - rc.Vertex[0].x, rc.Vertex[2].y - rc.Vertex[0].y, color, f);
			}
			else DrawRectangle(rc.Vertex[0].x, rc.Vertex[0].y, rc.Vertex[2].x, rc.Vertex[2].y, color, f, size);
		}

		void Draw_Shape(const Ellipse& el, COLORREF color, const double f, const double size = 0) {
			if (size == 0) {
				DrawEllipse(el.Vertex[0].x, el.Vertex[0].y, el.xRadius, el.yRadius, color, f);
			}
			else DrawEllipse(el.Vertex[0].x, el.Vertex[0].y, el.xRadius, el.yRadius, color, f, size);
		}

	};
}

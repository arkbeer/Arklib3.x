#pragma once
#include<wrl.h>
#include<atlbase.h>
#include<vector>
#include<wincodec.h>
#include<wincodecsdk.h>
#pragma comment(lib,"WindowsCodecs.lib")

namespace ark {
	class WIC {
		struct ComInitializer {
			ComInitializer() { CoInitialize(nullptr); }
			~ComInitializer() { CoUninitialize(); }
		};
		ComInitializer com;
		Microsoft::WRL::ComPtr<IWICImagingFactory2> IWICIFactory;
	public:
		class Image {
			std::vector<unsigned char> buffer;
			int width, height;
			int byte;
		public:
			Image(const int _width,const int _height, const int _byte):buffer(_width*_height*_byte),width(_width),height(_height),byte(_byte) {}
			unsigned char* getdata() {
				return buffer.data();
			}
			const unsigned char* getdata()const {
				return buffer.data();
			}
			const int getwidth()const {
				return width;
			}
			const int getheight()const {
				return height;
			}
			const int stride()const {
				return width*byte;
			}
			const size_t size()const {
				return buffer.size();
			}
		};
		WIC() {
			CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, &IWICIFactory);
		}
		~WIC() {
			IWICIFactory.ReleaseAndGetAddressOf();
		}
		Image loadimage(LPCTSTR imagename) {
			if (IWICIFactory) {
				Microsoft::WRL::ComPtr<IWICBitmapDecoder> dec;
				IWICIFactory->CreateDecoderFromFilename(imagename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &dec);
				Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
				dec->GetFrame(0, &frame);
				Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
				IWICIFactory->CreateFormatConverter(&converter);
				converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
				unsigned int width, height;
				frame->GetSize(&width, &height);
				Image im(width, height, 4);
				converter->CopyPixels(NULL, im.stride(), im.size(), im.getdata());
				return im;
			}
		}
	};
}
#pragma once
#include<xaudio2.h>
#pragma comment(lib,"Xaudio2.lib")
#include<fstream>
namespace ark {
	class Audio {
		template<typename T>
		class Com {
			T* ptr;
		public:
			Com() {
			}
			~Com() {
				if (ptr != nullptr) {
					ptr->DestroyVoice();
					ptr = nullptr;
				}
			}
			T** operator&() { return &ptr; }
			T** operator&()const { return &ptr; }
			T* operator->() { return ptr; }
			T* operator->()const { return ptr; }
			T* Get()const { return ptr; }
		};
	public:
		class WavSource {
			struct WavFileHeader {
				struct FmtChunk {
					std::uint32_t id;
					std::uint32_t bytes;
					std::uint16_t formatID;
					std::uint16_t channels;
					std::uint32_t rate;
					std::uint32_t velocity;
					std::uint16_t blocksize;
					std::uint16_t bits;
				};
				struct DataChunk {
					std::uint32_t id;
					std::uint32_t bytes;
				};
				std::uint32_t riffID;
				std::uint32_t size_8;
				std::uint32_t wavID;
				FmtChunk fmtchunk;
				DataChunk datachunk;
			};
			WavFileHeader head;
			WAVEFORMATEX wavex;
			std::unique_ptr<std::uint8_t[]> data;
			XAUDIO2_BUFFER buf;
			Com<IXAudio2SourceVoice> svoice;
		public:
			WavSource(const LPCTSTR lp) :head{}, wavex{}, buf{} {
				std::ifstream ifs;
				ifs.open(lp, std::ios::binary | std::ios::in);
				ifs.read(reinterpret_cast<char*>(&head), sizeof(head));
				wavex.cbSize = 0;
				wavex.wFormatTag = WAVE_FORMAT_PCM;
				wavex.nSamplesPerSec = head.fmtchunk.rate;
				wavex.wBitsPerSample = head.fmtchunk.bits;
				wavex.nChannels = head.fmtchunk.channels;
				wavex.nBlockAlign = head.fmtchunk.blocksize;
				wavex.nAvgBytesPerSec = head.fmtchunk.velocity;
				data = std::make_unique<std::uint8_t[]>(head.datachunk.bytes);
				ifs.read(reinterpret_cast<char*>(data.get()), head.datachunk.bytes);
				buf.AudioBytes = head.datachunk.bytes;
				buf.pAudioData = &data[0];
				buf.Flags = XAUDIO2_END_OF_STREAM;
			}
			WavSource(const LPCTSTR lp, Audio& a):WavSource(lp){
				Set_Audio(a);
			}
			~WavSource() {
				if (svoice.Get() != nullptr)svoice->Stop();
			}
			void Set_Audio(Audio& a) {
				a.XAudio->CreateSourceVoice(&svoice, &wavex);
			}
			void Play(const double s=0) {
				buf.PlayBegin = wavex.nSamplesPerSec*s;
				svoice->SubmitSourceBuffer(&buf);
				svoice->Start();
			}
			void Stop() {
				svoice->Stop();
			}
			WavSource& operator=(const WavSource& ws) {
				return *this;
			}
		};
	private:
		Microsoft::WRL::ComPtr<IXAudio2> XAudio;
		Com<IXAudio2MasteringVoice> MVoice;
		friend WavSource;
	public:
		Audio(){
			CoInitialize(nullptr);
			XAudio2Create(&XAudio);
			XAudio->CreateMasteringVoice(&MVoice);
		}
		~Audio(){
			CoUninitialize();
		}
	};
}
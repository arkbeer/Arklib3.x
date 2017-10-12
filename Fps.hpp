#pragma once
#include<chrono>
#include<thread>
namespace ark {
	class Fps {
		double fps;		
		using timepoint = std::chrono::time_point < std::chrono::steady_clock >;
		timepoint starttime;
		const double passed() {
			auto end = std::chrono::steady_clock::now();
			auto passed = end - starttime;
			return std::chrono::duration_cast<std::chrono::duration<double>>(passed).count();
		}
		const double needed() {
			return passed() - static_cast<double>(1 / fps);
		}
	public:

		Fps() :Fps(60.0) {}
		Fps(double _fps) :fps(_fps), starttime(std::chrono::steady_clock::now()) {}
        	auto Now(){
        	    return std::chrono::steady_clock::now();
        	}
		void SetFps(const double _fps){ fps=_fps; }
		void Count() {
			while (needed() < 0)std::this_thread::sleep_for(std::chrono::duration<int>(static_cast<int>(1 / fps)));
			starttime = std::chrono::steady_clock::now();
		}
		template<typename T>
		auto Duration(timepoint s) {
			return std::chrono::duration_cast<std::chrono::duration<T>>(std::chrono::steady_clock::now() - s).count();
		}
		template<typename T>
		auto perDuration(timepoint s) {
			return static_cast<T>(1/std::chrono::duration_cast<std::chrono::duration<T>>(std::chrono::steady_clock::now() - s).count());
		}

	};
}

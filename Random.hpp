#pragma once
namespace ark {
	class Random {
		std::mt19937 mt;
	public:
		Random() {
			std::random_device rnd;
			mt = std::mt19937(rnd());
		}
		const int Range(const int x, const int y) {
			auto it = std::minmax(x, y);
			std::uniform_int_distribution<> rand_(it.first, it.second);
			return rand_(mt);
		}
	};
}

#pragma once
namespace ark {
	class Key {
		std::array<bool, 256> keyup_arr;
		std::array<bool, 256> keydown_arr;

	public:
		Key() {
			keyup_arr.fill(false);
			keydown_arr.fill(false);
		}
		const bool KeyDown(const int key, const bool cond) {
			GetAsyncKeyState(key);
			if (GetAsyncKeyState(key)) {
				bool result;
				result = cond ? true : (cond == keydown_arr[key] ? true : false);
				keydown_arr[key] = true;
				return result;
			}
			else keydown_arr[key] = false;
			return false;
		}
		const bool KeyUp(const int key) {
			GetAsyncKeyState(key);
			if (GetAsyncKeyState(key)) {
				keyup_arr[key] = true;
				return false;
			}
			else {
				bool result;
				result = keyup_arr[key];
				keyup_arr[key] = false;
				return result;
			}
		}
	};
}

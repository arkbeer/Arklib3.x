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

ark::Fps fps;
ark::Random rnd;
ark::Key key;
int Main();
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	return Main();
}
enum class GameFlag {
	Title,Select,Main,Option,Credit
};
struct Notes {
	enum class State {
		None,Good,Miss
	};
	State state;
	enum class Type {
		Normal=0b0001,
		Head=0b0010,
		Tail=0b0100,
		Simul=0b1000
	};
	Type type;
	float time;
	float line;
	float y;
	float speed;
	Notes(const float _time, const float _speed, const float _line,Type _type) :time(_time), speed(_speed), line(_line), type(_type){
		y = line-fps.GetFps() * time*speed;
		state = State::None;
	
	}
	Notes(const float _time, const float _speed, const float _line):Notes(_time,_speed,_line,Type::Normal) {
	}
	void move() {
		y += speed;
	}
	const float diff() {
		return y - line;
	}
	const float dist() {
		return diff() > 0 ? diff() : -diff();
	}
	void init() {
		y = line - fps.GetFps() * time*speed;
		state = State::None;
	}
#define AS(STATE) void as_##STATE(){state=State::##STATE;}
	AS(None)
	AS(Good)
	AS(Miss)
#undef AS
#define IS(STATE) const bool is_##STATE() const{return state==State::##STATE;}
		IS(None)
		IS(Good)
		IS(Miss)
#undef IS
};
Notes::Type operator|(Notes::Type lt, Notes::Type rt) {
	return static_cast<Notes::Type>(static_cast<int>(lt) | static_cast<int>(rt));
}
Notes::Type operator&(Notes::Type lt, Notes::Type rt) {
	return static_cast<Notes::Type>(static_cast<int>(lt) & static_cast<int>(rt));
}
bool operator<<(Notes::Type lt, Notes::Type rt) {
	return (lt&rt) == rt;
}
auto readData(LPCTSTR lp) {
	std::vector<std::vector<std::string>> data;
	std::ifstream ifs;
	ifs.open(lp, std::ios::in);
	while (!ifs.eof()) {
		std::vector<std::string> v;
		std::string str;
		std::getline(ifs, str);
		std::stringstream ss;
		ss << str;
		while (std::getline(ss, str, ',')) {
			v.push_back(str);
		}
		data.push_back(v);
	}
	return data;
}
auto maketype(std::string str) {
	Notes::Type t;
	if (str.find('N') != std::string::npos) {
		t = Notes::Type::Normal;
	}
	else if (str.find('H') != std::string::npos) {
		t = Notes::Type::Head;
	}
	else if (str.find('T') != std::string::npos) {
		t = Notes::Type::Tail;
	}
	if (str.find('S') != std::string::npos) {
		t = t | Notes::Type::Simul;
	}
	return t;
}
auto makescore(const std::vector<std::vector<std::string>> data,const float speed,const float line) {
	std::vector<std::vector<Notes>> notes;
	for (const auto& vec2:data) {
		std::vector<Notes> n;
		for (const auto& vec:vec2) {
			std::stringstream ss;
			ss << vec;
			std::string str;
			std::vector<std::string> v;
			while (getline(ss, str, ':')) {
				v.push_back(str);
			}
			Notes::Type type=maketype(v[1]);
			

			n.push_back(Notes(std::atoi(v[0].c_str()), speed, line, type));
		}
		notes.push_back(n);
	}
	return notes;
}
int Main() {
	ark::WinClass wc;
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("v0.01")).SetSize(1200,800);
	ark::D2D1_1 d;
	GameFlag gameflag = GameFlag::Title;
	bool flag = false;
	float line = wc.GetSize().bottom*3.0 / 4;
	float width = wc.GetSize().right;
	std::string pushkey = "SDFJKL";
	auto check = readData(_T("test.txt"));
	auto score = makescore(readData(_T("test.txt")),6,line);
	std::array<std::vector<Notes>::iterator, 6> lanen = {
		score.begin()->begin() ,
		(score.begin() + 1)->begin() ,
		(score.begin() + 2)->begin(),
		(score.begin() + 3)->begin(),
		(score.begin() + 4)->begin(),
		(score.begin() + 5)->begin(),

	};
	auto s2 = std::chrono::steady_clock::now();
	d.Initialize();

	auto bmp = d.SetBitmap(L"icon.png");
	while (!wc.EndFlag()) {
		auto s = std::chrono::steady_clock::now();
		d.BeginDraw();
		wc.LockAspectRatio(16, 9);
		d.DrawClear(RGB(200,200,200));
		d.DrawBitmap(bmp, 0, 0, 100, 100, 0, 0, 100, 100, 1.0f);
		switch (gameflag) {
		case GameFlag::Title:
			d.DrawString(0,0,L"title",0x000000,1.0f,20);
			{
				bool startflag = false;
				for (int i = 0; i < 200; ++i) {
					if (key.KeyDown(i, false))startflag = true;
				}
				if (startflag) {
					gameflag = GameFlag::Main;
				}
			}
			break;
		case GameFlag::Select:
			d.DrawString(0, 0, L"select", 0x000000, 1.0f, 20);
			break;
		case GameFlag::Main:
			d.DrawLine(0, wc.GetSize().bottom*3.0 / 4, 1200, wc.GetSize().bottom*3.0 / 4, RGB(255, 255, 255), 1.0f, 2);
			for (auto it = score.begin(); it != score.end(); ++it) {
				for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
					const int n = std::distance(score.begin(), it);
					d.DrawEllipse(100 + n * 150, it2->y, 50, 50, RGB(0, 120, 255), 1.0f);
					d.DrawLine(50 + n * 150, it2->y, 150 + n * 150, it2->y, RGB(0, 0, 0), 1.0f, 2);
					if (it2->type << Notes::Type::Head) {
						OutputDebugString(_T("ya\n"));
						d.DrawRectangle(50 + n * 150, (it2 + 1)->y, 150 + n * 150, it2->y, RGB(0, 120, 255), 0.5f);

					}
				}
			}
			std::wstringstream time;
			time << (std::chrono::steady_clock::now() - s2).count();
			if (key.KeyDown(VK_RETURN, false)) {
				flag = !flag;
			}
			if (!flag) {
				s2 = std::chrono::steady_clock::now();
				for (auto& notes : score) {
					for (auto& v : notes) { v.init(); }
				}
				lanen = {
					score.begin()->begin() ,
					(score.begin() + 1)->begin() ,
					(score.begin() + 2)->begin(),
					(score.begin() + 3)->begin(),
					(score.begin() + 4)->begin(),
					(score.begin() + 5)->begin(),
				};

			}
			if (flag) {
				for (auto& notes : score) {
					for (auto& v : notes) {
						v.move();
					}
				}
			}
			for (int i = 0; i < lanen.size(); ++i) {
				auto& it = lanen.at(i);
				auto& lane = score.at(i);
				if (it != lane.end()) {
					if (it->diff() <= 100) {
						if (it->type != Notes::Type::Tail) {
							if (key.KeyDown(pushkey.at(i), false)) {
								if (it->dist() <= 100) {
									it->as_Good();
									++it;
								}
								else if (it->dist() <= 150) {
									it->as_Miss();
									++it;
								}
							}
						}
						else {
							if ((it - 1)->is_Good()) {
								if (key.KeyUp(pushkey.at(i))) {
									if (it->dist() <= 100) {
										it->as_Good();
										++it;
									}
									else {
										it->as_Miss();
										++it;
									}
								}
							}
							else {
								it->as_Miss();
								++it;
							}
						}
					}
					else {
						it->as_Miss();
						++it;
					}
				}
			}
			d.DrawString(0, 0, time.str().c_str(), 0x000000, 1.0f, 10);
			int c = 0;
			for (const auto& notes : score) {
				std::wstringstream result;
				for (const auto& v : notes) {
					if (v.is_Good())result << "Good\n";
					if (v.is_Miss())result << "Miss\n";
					if (v.is_None())result << "None\n";
				}
				d.DrawString(0 + c * 50, 10, result.str().c_str(), 0x000000, 1.0f, 10);
				c += 1;
			}
			d.DrawString(0, 100, L"(re)start:enter\nkey:S,D,F,J,K,L", 0x000000, 1.0f, 10);
			break;
		}
		d.EndDraw();
		fps.Count();
		ark::TstringStream tstr;
		tstr << fps.perDuration<double>(s)<<"\n";
		OutputDebugString(tstr.str().c_str());
	}
	return 0;
}

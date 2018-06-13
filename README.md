# ArkLib Reference
これは開発中のArklibの備忘録かつリファレンスとなるものである。以下は各ファイルを箇条書きにしているだけで有り、順番は適当である。

## 0. 設計思想
本ライブラリはwinapiのラッパーライブラリであるため、windows上で動くことを想定しており、その他の環境には対応していない。また、directXにも依存しており、windows7以前のPCでは動かないと思われる。
本ライブラリは公開されているその他のライブラリや、自作のライブラリの機能の不足分を補充できるように、複数のヘッダ(hpp)と一つのcppファイルからなるものであり、使いたい機能に応じてヘッダを追加していく形式をとっており、全てのヘッダをなるべく独立させ、ヘッダごとの依存のないように設計している。
また、各クラス、関数は名前空間arkの中に存在しているものとし、極力グローバル空間を汚さない設計としている。
## 1. Main.cpp
### 概要
メインとなるソースコードを記入する為のcppファイル
ここにwinMain等を書く用に設計しているが、別段このファイルである必要性はない

### サンプル
```=c++
#include<iostream>
#include<string>
#include"WinMain.hpp"
#include"Fps.hpp"
ark::Fps fps;
ark::WinClass wc;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("Win32API Example")).SetSize(640, 480);
	while (wc.Loop()) {
		auto s = std::chrono::steady_clock::now();
		fps.Count();
	}
	return 0;
}
```
## 2. Key.hpp
### 概要
windowsにおけるキー制御の為のクラス

### クラス
- Key
    - KeyDown
        ```=c++
        const bool KeyDown(const int key, const bool cond = true)
        ```
        キーが押されていればtrue, 押されていなければfalseを返す
        condがtrueの時押しっぱなしで有効、falseで無効
    - KeyUp
        ```=c++
        const bool KeyUp(const int key)
        ```
        キーが離されたときにtrue,それ以外のときにfalseを返す

## 3. Tstring.hpp
### 概要
windowsの文字コード問題に対応するためのヘルパー

### using宣言
- Tstring
    std::stringのtchar版
- TstringStream
    std::stringstreamのtchar版

## 4. Random.hpp
### 概要
mt19937による乱数生成クラス

### クラス
- Random
    - Range
        ```=c++
        const int Range(const int x, const int y)
        ```
        xからyまでの間で乱数を発生させ、返す

## 5. Mouse.hpp
### 概要
マウス座標を取得する関数

### 関数
- GetScreenPosition
    ```c++
    auto GetScreenPosition() -> std::pair<int, int>
    ```
    マウスのスクリーン座標を返す
- GetClientPosition
    ```c++
    auto GetClientPosition(HWND hWnd) -> std::pair<int, int>
    ```
    マウスのHWNDで指定されたクライアントでの座標を返す

## 6. Fps.hpp
### 概要
FPS制御を行います

### クラス
- Fps
    ```c++
    Fps():Fps(60.0)
    Fps(const double _fps)
    ```
    fpsの上限を設定可能、デフォルトは60

    - Now
        ```c++
        auto Now() -> std::chrono::time_point < std::chrono::steady_clock >
        ```
        現在の時間を取得する
    - SetFps
        ```c++
        void SetFps(const double _fps)
        ```
        fpsの上限を設定する
    - Count
        ```c++
        void Count()
        ```
        fpsを計測、また一定に保つための処理を行う
    - Duration
        ```c++
        template<typename T>
		auto Duration(timepoint s)
        ```
        引数から経過した時間をT型で返す
        
    - PerDuration
        ```c++
        template<typename T>
		auto perDuration(timepoint s)
        ```
        引数から経過した時間からfpsを測定し、T型で返す
        
## 7. WinMain.hpp
### 概要
ウィンドウを作成します

### クラス
- WinClass
    ```c++
    WinClass():WinClass(Default_WNDCLASSEX())
    WinClass(WNDCLASSEX wex)
    ```
    - Get Functions
        ```c++
        HWND GetHwnd()
        ```
        HWNDを返す
        
        ```c++
        RECT GetSize()
        ```
        ウィンドウの表示領域のサイズを返す
        
        ```c++
        WinMainArguments GetWinMainArguments()
        ```
        WinMainの引数を返す
    - Set / Add Functions
        ```c++
        WinClass& SetStyle(LONG)
        WinClass& SetExStyle(LONG)
        WinClass& SetCursor(LONG)
        WinClass& SetIcon(LONG)
        WinClass& SetIconSM(LONG)
        WinClass& SetTitle(LPCTSTR lp)
        WinClass& SetSize(int x,int y)
        const bool SetWindowElements(int index, LONG lp)
        const bool SetWndClassExElements(int index, LONG lp)
        const bool SetWindowPosition(HWND z, int x, int y, int cx, int cy, UINT uf = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW)
        ```
        各パラメータを設定する関数
        ```c++
        WinClass& GetStyle(LONG)
        WinClass& GetExStyle(LONG)
        WinClass& GetCursor(LONG)
        WinClass& GetIcon(LONG)
        WinClass& GetIconSM(LONG)
        ```
        各パラメータ要素を取得する関数
        
        ```c++
        WinClass& LockAspectRatio(const float xratio, const float yratio)
        ```
        アスペクト比を設定する関数
        
        ```c++
        const bool Loop()
        ```
        ループさせる関数
## 8. D2D1_1.hpp
### 概要
描画をDirect2D1.1を使って行います

### クラス
- D2D1_1
    ```c++
    D2D1_1()
    //no option
    ```
    

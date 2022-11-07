//インクルード
#include <Windows.h>
#include <stdlib.h>
#include"Engine/Camera.h"
#include"Engine/Direct3D.h"
#include"Engine/Input.h"
#include"Engine/RootJob.h"
#include"Engine/Model.h"
#include"Engine/Macro.h"


#pragma comment(lib, "winmm.lib")

#pragma region //定数宣言

LPCSTR WIN_CLASS_NAME = "SampleGame";  //ウィンドウクラス名
const int WINDOW_WIDTH = 1280;            //ウィンドウの幅
const int WINDOW_HEIGHT = 720;			  //ウィンドウの高さ

#pragma endregion

#pragma region //プロトタイプ宣言

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma endregion

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
#pragma region //ウィンドウクラス（設計図）を作成

	WNDCLASSEX wc;//構造体

	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）

	RegisterClassEx(&wc);  //クラスを登録

#pragma endregion
	
#pragma region //ウィンドウサイズの計算

	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);  //ウィンドウサイズをいい感じにしてくれる
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

#pragma endregion

#pragma region //ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		"サンプルゲーム",     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                 //ウィンドウ幅
		winH,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);
#pragma endregion

//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

//comの初期化
	HRESULT hr;
	hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		PostQuitMessage(0);
	}


//Direct3D初期化
	
	hr = Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd);
	if (FAILED(hr))
	{
		PostQuitMessage(0);
	}
	
	//Input
	hr = Input::Initialize(hWnd);
	if (FAILED(hr))
	{
		PostQuitMessage(0);
	}

	Camera::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//RootJob
	RootJob* pRootJob_ = nullptr;
	pRootJob_ = new RootJob(nullptr);
	pRootJob_->Initialize();

	XMVECTOR initial_camposition = Camera::GetPosition();
	XMVECTOR initial_camtraget = Camera::GetTarget();
#pragma region //メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			timeBeginPeriod(1);//この間は、ちゃんと1ミリ秒単位で時間を計測してくださいという意味
			static DWORD countFps = 0;//何回画面を更新したか
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();//Windowsが起動してからの時間をミリ秒で取得できる
			static DWORD lastUpdateTime = nowTime;
			

			if (nowTime - startTime>= 1000)//1秒間の更新回数を求めたい
			{
				char str[16];
				wsprintf(str, "%u", countFps);//更新回数を表示するのは1秒に1回
				SetWindowText(hWnd, str);//ウィンドウのキャプションバー（タイトルバー）に表示

				countFps = 0;
				startTime = nowTime;
			}
			if ((nowTime - lastUpdateTime)*60 <= 1000.0f)//画面の更新を1/60秒に1回に抑えたい
			{
				continue;
			}
			lastUpdateTime = nowTime;

			countFps++;

			
			//ゲームの処理
			
			//描画処理
			Camera::Update();
			Input::Update();

			Direct3D::BeginDraw();

			pRootJob_->UpdateSub();
			pRootJob_->DrawSub();
			

			Direct3D::EndDraw();

			timeEndPeriod(1);//ここまで正確に測る
		}
	}
	Model::AllRelease();
	Input::Release();
	Direct3D::Release();
	pRootJob_->ReleaseSub();
	SAFE_DELETE(pRootJob_);
	CoUninitialize();
	return 0;
#pragma endregion

}

#pragma region //ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)//結果を返す
{
	switch (msg)//メッセージが入って区別する
	{
	case WM_DESTROY://ウィンドウが削除された
		PostQuitMessage(0);  //プログラム終了
		return 0;
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#pragma endregion



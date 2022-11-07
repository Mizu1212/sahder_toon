#pragma once
#include <dInput.h>
#include <DirectXMath.h>
#include "XInput.h"
#include"Macro.h"

//リンカ
#pragma comment(lib,"Xinput.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")



using namespace DirectX;

namespace Input
{
	//初期化
	//引数：hWnd	ウィンドウハンドル
	HRESULT Initialize(HWND hWnd);//初期化
	void Update();//更新


	///////////////////////////　キーボード　//////////////////////////////////
	//キーが押されているか調べる
	//引数：keyCode	調べたいキーのコード
	//戻値：押されていればtrue
	bool IsKey(int keyCode);

	//キーを今押したか調べる（押しっぱなしは無効）
	//引数：keyCode	調べたいキーのコード
	//戻値：押した瞬間だったらtrue
	bool IsKeyDown(int keyCode);

	//キーを今放したか調べる
	//引数：keyCode	調べたいキーのコード
	//戻値：放した瞬間だったらtrue
	bool IsKeyUp(int keyCode);//さっきは押してて、今は押してない状態

	///////////////////////////　マウス　//////////////////////////////////
	//マウスのボタンが押されているか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押されていればtrue
	bool IsMouseButton(int buttonCode);

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押した瞬間だったらtrue
	bool IsMouseButtonDown(int buttonCode);

	//マウスのボタンを今放したか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：放した瞬間だったらtrue
	bool IsMouseButtonUp(int buttonCode);


	//マウスカーソルの位置を取得
	//戻値：マウスカーソルの位置
	XMFLOAT3 GetMousePosition();

	//マウスカーソルの位置をセット
	//引数：マウスカーソルの位置
	void SetMousePosition(int x, int y);

	//そのフレームでのマウスの移動量を取得
	//戻値：X,Y マウスの移動量　／　Z,ホイールの回転量
	XMFLOAT3 GetMouseMove();

	///////////////////////////　コントローラー　//////////////////////////////////
	//コントローラーのボタンが押されているか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押されていればtrue
	bool IsPadButton(int buttonCode, int padID = 0);

	//コントローラーのボタンを今押したか調べる（押しっぱなしは無効）
	//引数：buttonCode	調べたいボタンの番号
	//戻値：押した瞬間だったらtrue
	bool IsPadButtonDown(int buttonCode, int padID = 0);

	//コントローラーのボタンを今放したか調べる
	//引数：buttonCode	調べたいボタンの番号
	//戻値：放した瞬間だったらtrue
	bool IsPadButtonUp(int buttonCode, int padID = 0);

	//左スティックの傾きを取得
	//戻値:傾き具合（-1～1）
	XMFLOAT3 GetPadStickL(int padID = 0);

	//右スティックの傾きを取得
	//戻値:傾き具合（-1～1）
	XMFLOAT3 GetPadStickR(int padID = 0);

	//左トリガーの押し込み具合を取得
	//戻値:押し込み具合（0～1）
	float		GetPadTrrigerL(int padID = 0);

	//右トリガーの押し込み具合を取得
	//戻値:押し込み具合（0～1）
	float		GetPadTrrigerR(int padID = 0);

	//振動させる
	void SetPadVibration(int l, int r, int padID = 0);


	void Release();//解放
};
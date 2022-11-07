#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	//引数 第一引数ウィンドウの幅、第二引数ウィンドウの縦
	void Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT);

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	//引数　第一引数カメラの位置
	void SetPosition(XMVECTOR position);

	//焦点（見る位置）を設定
	//引数　第一引数焦点の位置
	void SetTarget(XMVECTOR target);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//視点（カメラの位置）を取得
	XMVECTOR GetPosition();

	//焦点（見る位置）を取得
	XMVECTOR GetTarget();
};
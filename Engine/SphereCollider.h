#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include"Transform.h"
using namespace DirectX;

//-----------------------------------------------------------
//球体の当たり判定
//-----------------------------------------------------------



class SphereCollider
{
protected:
	//GameObject* pGameObject_;	//この判定をつけたゲームオブジェクト
	XMFLOAT3		center_;		//中心位置（ゲームオブジェクトの原点から見た位置）
	float radius;

	//int				hDebugModel_;	//デバッグ表示用のモデルのID
public:
	//コンストラクタ（当たり判定の作成）
	//引数：center	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：radius	当たり判定のサイズ（半径）
	SphereCollider(XMFLOAT3 center, float radius_);

	
	//コライダーを取得
	float GetCollider();
};


#include "SphereCollider.h"

//コンストラクタ（当たり判定の作成）
//引数：center	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：radius	当たり判定のサイズ（半径）
SphereCollider::SphereCollider(XMFLOAT3 center, float radius_)
{
	center_ = center;
	radius = radius_;
}

float SphereCollider::GetCollider()
{
	return radius;
}



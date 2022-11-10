#include "Camera.h"

namespace Camera
{
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
};

//初期化
void Camera::Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	position_ = XMVectorSet(0, 0, -5, 0);	//カメラの位置
	target_ = XMVectorSet(0, -2, 0, 0);	//カメラの焦点

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);//1画角２縦横の比率３手前の表示範囲4どこまで描画するか
}

//更新
void Camera::Update()
{

	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

//位置を設定
void Camera::SetPosition(XMVECTOR position)
{
	 position_  = position;
}

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

//位置を取得
XMVECTOR Camera::GetPosition()
{
	return position_;
}

//焦点を取得
XMVECTOR Camera::GetTarget()
{
	return target_;
}

#include "Transform.h"

Transform::Transform()
{
	matTranslate_ = XMMatrixIdentity();
	matRotate_ = XMMatrixIdentity();
	matScale_ = XMMatrixIdentity();
	position_ = XMFLOAT3(0,0,0);
	rotate_ = XMFLOAT3(0, 0, 0);
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	pParent_ = nullptr;
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
	matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x)) * XMMatrixRotationY(XMConvertToRadians(rotate_.y)) * XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
	Calclation();

	if (pParent_==nullptr)
	{
		return (matScale_ * matRotate_ * matTranslate_);
	}
	return  (matScale_ * matRotate_ * matTranslate_) * pParent_->GetWorldMatrix();
}

XMMATRIX Transform::GetNormalMatrix()
{
	return XMMatrixInverse(nullptr, matScale_) * matRotate_;//–@ü—p‚Ìs—ñ
}

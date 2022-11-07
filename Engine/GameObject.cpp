#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(GameObject* parent)
{
	pParent_ = nullptr;
	objectName_ = "";
}

GameObject::GameObject(GameObject* parent, const std::string& name)
{
	pParent_  = parent;
	objectName_ = name;
	if(parent!=nullptr)
	transform_.pParent_ = &parent->transform_;
}

GameObject::~GameObject()
{
}

//子オブジェクトリストを取得
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

void GameObject::DrawSub()
{
	Draw();

	for (auto i = childList_.begin();i != childList_.end();i++)
	{
		(*i)->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	Update();
	Oncollision(GetRootJob());

	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->UpdateSub();
		
	}
	for (auto i = childList_.begin(); i != childList_.end();)
	{
		if ((*i)->IsDed())
		{
			(*i)->ReleaseSub();
			delete(*i);
			i = childList_.erase(i);
		}
		else
		{
			i++;
		}

	}
}

void GameObject::ReleaseSub()
{
	Release();
    for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
			(*i)->ReleaseSub();
	}
}

XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform_.GetWorldMatrix();
	
}

bool GameObject::IsDed()
{
	return deadf ;
}

void GameObject::KillMe()
{
	deadf = true;
}

//子オブジェクトを全て削除
void GameObject::KillAllChildren(void)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトを1個ずつ削除
	while (it != end)
	{
		KillObjectSub(*it);
		delete* it;
		it = childList_.erase(it);
	}

	//リストをクリア
	childList_.clear();
}
//名前でオブジェクトを検索（対象は自分の子供以下）
GameObject* GameObject::FindChildObject(const std::string& name)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return nullptr;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトから探す
	while (it != end) {
		//同じ名前のオブジェクトを見つけたらそれを返す
		if ((*it)->GetObjectName() == name)
			return *it;

		//その子供（孫）以降にいないか探す
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//次の子へ
		it++;
	}

	//見つからなかった
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}

//親オブジェクトを取得
GameObject* GameObject::GetParent(void)
{
	return pParent_;
}

//オブジェクトの名前を取得
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

//総当たり
void GameObject::Oncollision(GameObject* pTarget)
{
	Collision(pTarget);
	for (auto i = pTarget->GetChildList()->begin(); i != pTarget->GetChildList()->end(); i++)
	{
		Oncollision(*i);
	}
}

//void GameObject::AddCollider(SphereCollider* collider)
//{
//	collider->SetGameObject(this);
//	scolliderList_.push_back(collider);
//}

//衝突判定
void GameObject::Collision(GameObject* pTarget)
{
	//自分同士の当たり判定はしない
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	float x =0.0;
	float y = 0.0;
	float z = 0.0;
	float K = 0.0;
	float L = 0.0;
	x = this->transform_.position_.x - pTarget->transform_.position_.x;
	y = this->transform_.position_.y - pTarget->transform_.position_.y;
	z = this->transform_.position_.z - pTarget->transform_.position_.z;
	K = (x * x + y * y + z * z);
	if (this->pSphereCollider_ != nullptr&& pTarget->pSphereCollider_!=nullptr)
	{
		L = this->pSphereCollider_->GetCollider() + pTarget->pSphereCollider_->GetCollider();
	}
	else
	{
		return;
	}

	if (K<=(L*L))
	{
		OnCollision(pTarget);
	}
}

//オブジェクト削除（再帰）
void GameObject::KillObjectSub(GameObject* obj)
{
	if (!childList_.empty())
	{
		auto list = obj->GetChildList();
		auto it = list->begin();
		auto end = list->end();
		while (it != end)
		{
			KillObjectSub(*it);
			delete* it;
			it = list->erase(it);
		}
		list->clear();
	}
	obj->Release();
}
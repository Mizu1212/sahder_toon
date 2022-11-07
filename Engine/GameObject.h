#pragma once
#include <string>
#include <list>
#include "Transform.h"
#include"SphereCollider.h"



class GameObject
{
private:
	bool deadf = false; //死んでいるかのフラグ
protected:
	std::list<GameObject*> childList_;//ゲームオブジェクトのリスト構造
	Transform	transform_;//transformクラス
	GameObject* pParent_;//ゲームオブジェクトなら誰でも（ポインタ）
	std::string	objectName_;//文字列
	SphereCollider* pSphereCollider_;//SphereCollider型のポインタ

	

public:
	//コンストラクタ
	GameObject();
	//引数付きコンストラクタ
	//引数　第一引数GameObject型のポインタ
	GameObject(GameObject* parent);
	//引数付きコンストラクタ
	//引数　第一引数GameObject型のポインタ　第二引数　string型のポインタ
	GameObject(GameObject* parent, const std::string& name);
	//デストラクタ
	virtual ~GameObject();


	virtual void Initialize() = 0;//純粋仮想関数
	virtual void Update() = 0;    //純粋仮想関数
	virtual void Draw() = 0;      //純粋仮想関数
	virtual void Release() = 0;   //純粋仮想関数
	
	//テンプレート
	template <class T>
	void Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);
	}

	template <class T>
	GameObject* PInstantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);

		return p;
	}

	//子オブジェクトリストを取得
	std::list<GameObject*>* GetChildList();

	//ポジションのセット
	void SetPosition(XMFLOAT3 position);
	
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();
	
	XMMATRIX GetWorldMatrix();

	//死んでいるかを見る
	bool IsDed();

	//削除
	void KillMe();

	//子オブジェクトを全て削除
	void KillAllChildren();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数　第一引数string型のポインタ
	GameObject* FindChildObject(const std::string& name);

	//名前でオブジェクトを検索（対象は全体）
	//引数　第一引数string型のポインタ
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//RootJobを取得
	GameObject* GetRootJob();

	//親オブジェクトを取得
	GameObject* GetParent();

	//オブジェクトの名前を取得
	const std::string& GetObjectName(void) const;


	//XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }


	//コライダー（衝突判定）を追加する
	//引数　第一引数SphereCollider型のポインタ
	void AddCollider(SphereCollider* collider) { pSphereCollider_ = collider; }

	//総当たり
	//引数　第一引数GameObject型のポインタ
	void Oncollision(GameObject* pTarget);

	//何かと衝突した場合に呼ばれる（オーバーライド用）
	//引数：pTarget	衝突した相手
	virtual void OnCollision(GameObject* pTarget) {};

	//衝突判定
	//引数：pTarget	衝突してるか調べる相手
	void Collision(GameObject* pTarget);
private:
	//オブジェクト削除（再帰）
	void KillObjectSub(GameObject* obj);
};
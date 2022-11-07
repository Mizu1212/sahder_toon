#pragma once
#include "GameObject.h"
class RootJob :
    public GameObject
{
public:
	//コンストラクタ
	//引数　GameObject型のポインタ
	RootJob(GameObject* parent);
	~RootJob();
	 void Initialize();
	 void Update();
	 void Draw();
	 void Release();
};


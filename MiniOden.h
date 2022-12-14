#pragma once
#include "Engine/GameObject.h"
class MiniOden :
    public GameObject
{
    int hModel_;
public:
    //コンストラクタ
    MiniOden(GameObject* parent);

    //デストラクタ
    ~MiniOden();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};


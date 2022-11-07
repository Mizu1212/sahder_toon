#include "GameStartScene.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include"Engine/Input.h"
//コンストラクタ
GameStartScene::GameStartScene(GameObject* parent)
	: GameObject(parent, "GameStartScene"), hPict_(-1)
{
}

//初期化
void GameStartScene::Initialize()
{
	//画像データのロード
	hPict_ = Image::Load("Assets\\GameStart.png");
	assert(hPict_ >= 0);
}

//更新
void GameStartScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PlayScene);
	}
}

//描画
void GameStartScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//開放
void GameStartScene::Release()
{
}
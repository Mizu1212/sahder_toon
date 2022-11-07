#include "GameOver.h"
#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Image.h"


//コンストラクタ
GameOver::GameOver(GameObject* parent)
	: GameObject(parent, "GameOver"), hPict_(-1)
{
}

//初期化
void GameOver::Initialize()
{
	//画像データのロード
	hPict_ = Image::Load("Assets\\Kinniku.jpg");
	assert(hPict_ >= 0);
}

//更新
void GameOver::Update()
{
	
}

//描画
void GameOver::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

//開放
void GameOver::Release()
{
}
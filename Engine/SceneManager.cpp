#include "SceneManager.h"
#include "../PlayScene.h"
#include"../GameOver.h"
#include"../GameStartScene.h"
#include "Model.h"

//コンストラクタ
SceneManager::SceneManager(GameObject* parent)
	: GameObject(parent, "SceneManager")
{
}
//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
	firstSceneID_ = SCENE_ID_GameStart;
	nextSceneID_ = firstSceneID_;
	Instantiate<GameStartScene>(this);
}
//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (firstSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();
		//ロードしたデータを全削除
		Model::AllRelease();

		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_PlayScene: Instantiate<PlayScene>(this); break;
		case SCENE_ID_GameOver: Instantiate<GameOver>(this); break;
		case SCENE_ID_GameStart: Instantiate<GameStartScene>(this); break;
		}
		firstSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}
#pragma once
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_PlayScene = 0,
	SCENE_ID_GameOver,
	SCENE_ID_GameStart,
};

//シーン切り替えを担当するオブジェクト
class SceneManager : public GameObject
{
public:
	//コンストラクタ
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	void ChangeScene(SCENE_ID next);

private:
	SCENE_ID firstSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

};
#pragma once
#include<vector>
#include<string>
#include"Spirete.h"
#include"Transform.h"

namespace Image
{

	struct ImageData
	{
		Spirete* pSpirete;      //Spirete型のポインタ

		std::string filename_; //ファイルの名前

		Transform transform_;  //transfrom

		//コンストラクタ
		ImageData() : pSpirete(nullptr)
		{
		};

	};

	//初期化
	void Initialize();

	//更新
	int Load(std::string fileName);

	//描画
	void Draw(int handle);

	//開放
	void Release(int handle);//初期化

	//transformの設定
	void SetTransform(int handle, Transform& transform_);
	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle);
	//すべて消す
	void AllRelease();
}


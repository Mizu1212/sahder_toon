#include "Image.h"
#include"Direct3D.h"
namespace Image
{
	//ロード済みのモデルデータ一覧
	std::vector<ImageData*>vi_Data;

	//初期化
	void Initialize()
	{
		AllRelease(); //すべて開放
	}

	//モデルをロード
	int Load(std::string fileName)
	{
		ImageData* pIData = new ImageData;//構造体のインスタンスを作成

		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;//同じファイルがあるかどうか見る Exist=存在
		for (int i = 0; i < vi_Data.size(); i++)
		{
			//すでに開いている場合
			if (vi_Data[i] != nullptr && vi_Data[i]->filename_ == fileName)
			{
				pIData->pSpirete = vi_Data[i]->pSpirete;
				isExist = true;
				break;
			}
		}
		
		//新たにファイルを開く
		if (isExist == false)
		{
			pIData->pSpirete = new Spirete;

			wchar_t wtext[FILENAME_MAX];
			size_t ret;
			mbstowcs_s(&ret, wtext, fileName.c_str(), strlen(fileName.c_str()));//マルチバイトからワイド変更
			if (FAILED(pIData->pSpirete->Load(wtext)))
			{
				//開けなかった
				SAFE_DELETE(pIData->pSpirete);
				SAFE_DELETE(pIData);
				return -1;
			}
			//無事開けた
			pIData->filename_ = fileName;
		}

		//新たに追加
		vi_Data.push_back(pIData); //構造体の中身が埋まったので動的配列に突っ込む
		return (int)vi_Data.size() - 1;//番号（配列の要素数-1）を返す
	}

	void Draw(int handle)
	{
		//モデルの確認
		if (handle < 0 || handle >= vi_Data.size() || vi_Data[handle] == nullptr)
		{
			return;
		}

		if (vi_Data[handle]->pSpirete)
		{
			vi_Data[handle]->pSpirete->Draw(vi_Data[handle]->transform_,800,600);
		}
	}

	void Release(int handle)
	{
		//モデルの確認
		if (handle < 0 || handle >= vi_Data.size() || vi_Data[handle] == nullptr)
		{
			return;
		}
		//同じモデルを他でも使っていないか
		bool isExist = false;//使ってないかの存在確認
		for (int i = 0; i < vi_Data.size(); i++)
		{
			//すでに開いている場合
			if (vi_Data[i] != nullptr && i != handle && vi_Data[i]->pSpirete == vi_Data[handle]->pSpirete)
			{
				isExist = true;
				break;
			}
		}
		//使ってなければモデル解放
		if (isExist == false)
		{
			SAFE_DELETE(vi_Data[handle]->pSpirete);
		}


		SAFE_DELETE(vi_Data[handle]);
	}

	void SetTransform(int handle, Transform& transform_)
	{
		if (handle < 0 || handle >= vi_Data.size())
		{
			return;
		}

		vi_Data[handle]->transform_ = transform_;
	}

	XMMATRIX GetMatrix(int handle)
	{
		return vi_Data[handle]->transform_.GetWorldMatrix();
	}

	void AllRelease()
	{
		for (int i = 0; i < vi_Data.size(); i++)
		{
			if (vi_Data[i] != nullptr)
			{
				Release(i);
			}
		}
		vi_Data.clear();
	}
}














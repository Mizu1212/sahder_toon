#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include"Transform.h"

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}
//2D画像を描画するクラス
class Spirete
{
protected:
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		//ワールド行列
		XMMATRIX	matW;

	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//UV

	};
	DWORD	vertexNum_;		//頂点数
	VERTEX* vertices_;		//頂点情報
	ID3D11Buffer* pVertexBuffer_;		//頂点バッファ
	DWORD indexNum;			//インデックス数
	int* index_;			//インデックス情報
	ID3D11Buffer* pIndexBuffer_;		//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;		//テクスチャ
	UINT WIDTH;             //ウィンドウのサイズ
	UINT HEIGHT;            //ウィンドウのサイズ

public:
	Spirete();
	~Spirete();
	
	//初期化（ポリゴンを表示するための各種情報を準備）
	//戻値：成功／失敗
	HRESULT Load(LPCWSTR fileName);
	//描画
	//引数：worldMatrix	ワールド行列
	void Draw(Transform transform, int WINDOW_WIDTH, int WINDOW_HEIGHT);
	//解放
	void Release();
private:
	//---------Initializeから呼ばれる関数---------
	virtual void InitVertexData();		//頂点情報の準備
	HRESULT CreateVertexBuffer();		//頂点バッファを作成
	virtual void InitIndexData();		//インデックス情報を準備
	HRESULT CreateIndexBuffer();		//インデックスバッファを作成
	HRESULT CreateConstantBuffer();		//コンスタントバッファ作成

	HRESULT LoadTexture(LPCWSTR fileName);				//テクスチャをロード
	//---------Draw関数から呼ばれる関数---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix,int WINDOW_WIDTH,int WINDOW_HEIGHT);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();							//各バッファをパイプラインにセット
};
#pragma once
#include <d3d11.h>
#include "string"

class Texture
{
	ID3D11SamplerState* pSampler_;  //テクスチャの貼り方を決める
	ID3D11ShaderResourceView* pSRV_;//テクスチャをシェーダーに渡すためのビューを作成する
	UINT imgWidth;
	UINT imgHeight;
public:
	Texture();
	~Texture();
	HRESULT Load(LPCWSTR fileName);
	void Release();
	UINT GetWidth(){return imgWidth; }//高さ取得
	UINT GetHeight() { return imgHeight; }//幅取得
	ID3D11SamplerState* GetSampler() { return pSampler_; }//サンプラー取得
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }//ビュー取得
};
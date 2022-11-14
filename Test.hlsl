Texture2D tex : register(t0); //どのテクスチャを使うかを入れる
SamplerState smp : register(s0);


cbuffer gloabl
{
	float4x4 matWVP;   //行列を扱う型
	float4   color;    //マテリアルの色
	bool     isTexture;//テクスチャが貼られているかどうか
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};

//頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)//:セマンティクスは何の情報か　
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //ベクトルを行列にする関数
	outData.uv = uv;
	return outData;
}

//ピクセルシェーダー オブジェクトに関係なしに色を決めれる
float4 PS(VS_OUT inData) : SV_TARGET //SVは二次元 ピクセルシェーダーの引数は頂点シェーダーの戻り値と同じ
{
	if (isTexture)
	{
		return tex.Sample(smp, inData.uv);
	}
    else
    {
		return color;
    }
}
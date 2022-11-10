cbuffer gloabl
{
	float4x4 matW;   //行列を扱う型
};



//頂点シェーダー
float4 VS(float4 pos : POSITION) : SV_POSITION//:セマンティクスは何の情報か　
{
	pos = mul(pos,matW);   //ベクトルを行列にする関数
	return pos;
}

//ピクセルシェーダー オブジェクトに関係なしに色を決めれる
float4 PS(float4 pos : SV_POSITION) : SV_TARGET //SVは二次元
{
	
	return float4(1-(pos.x/800), 1, 1, 1);
}
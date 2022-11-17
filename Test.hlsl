Texture2D tex : register(t0); //どのテクスチャを使うかを入れる
SamplerState smp : register(s0);


cbuffer gloabl
{
	float4x4 matWVP;   //行列を扱う型
	float4x4 matNormal;
	float4x4 matW;
	float4   color;    //マテリアルの色
	float4   camPos;
	bool     isTexture;//テクスチャが貼られているかどうか
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 color : COLOR0;
	float4 specular : COLOR1;
};

//頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)//:セマンティクスは何の情報か　
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //ベクトルを行列にする関数
	outData.uv = uv;

	float4 light = float4(1, 1, -1, 0);
	light = normalize(light);
	normal = mul(normal, matNormal);//法線回転
	outData.color = dot(normal, light);
	outData.color = clamp(outData.color, 0, 1); //切り詰め
	
	float4 V = normalize(mul(pos, matW) - camPos);//視点から
	float4 R = reflect(light, normal);
	
	//outData.specular = pow(clamp(dot(R, V),0,1), 200) * 1;
	outData.specular = clamp(dot(R, V),0,1);
	outData.specular = pow(outData.specular, 10);

	return outData;
}

//ピクセルシェーダー オブジェクトに関係なしに色を決めれる
float4 PS(VS_OUT inData) : SV_TARGET //SVは二次元 ピクセルシェーダーの引数は頂点シェーダーの戻り値と同じ
{

	float4 diffuse;
    float4 ambient;


	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv)*inData.color;
		ambient = tex.Sample(smp, inData.uv) * 0.3;
	}
    else
    {
		diffuse = color*inData.color;
		ambient = color * 0.3;
    }
	return /*diffuse + ambient +*/ inData.specular;
}
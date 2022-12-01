//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{

	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matNormal;	        //法線変更用行列
	float4		diffuseColor;		// ディフューズカラー（マテリアルの色）
	bool		isTexture;		// テクスチャ貼ってあるかどうか

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;	        //UV座標
	float4 color	: COLOR;	//色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報

	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて

	//スクリーン座標に変換し、ピクセルシェーダーへ

	outData.pos = mul(pos, matWVP);
	outData.uv = uv;


	//法線を回転
	normal = mul(normal, matNormal);//nomalにmatWをかける

	float4 light = float4(0,1,-1,0);//光の進む方向とは逆のベクトル
	light = normalize(light);//正規化
	outData.color = clamp(dot(normal, light), 0, 1);//dotは内積を求める第一引数を第二引数、第三引数の間の数にする


	//まとめて出力

	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	if (isTexture) {
	float4 diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;//拡散反射光
	float4 ambient = g_texture.Sample(g_sampler, inData.uv) * float4(0.4, 0.4, 0.4, 1);//環境光
	return diffuse + ambient;
	}
    else
	{
		return diffuseColor * inData.color;
    }
}

//輪郭表示用頂点シェーダー
float4 VS_Outline(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) : SV_POSITION
{
	pos = mul(pos, matWVP);
	return pos;
}

//輪郭表示用のピクセルシェーダー
float4 PS_Outline(VS_OUT inData) : SV_Target
{
	return float4 (0,0,0,1);
}
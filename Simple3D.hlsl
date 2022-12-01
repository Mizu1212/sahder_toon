//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{

	float4x4	matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	matNormal;	        //�@���ύX�p�s��
	float4		diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	bool		isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���

};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	        //UV���W
	float4 color	: COLOR;	//�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����

	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������

	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��

	outData.pos = mul(pos, matWVP);
	outData.uv = uv;


	//�@������]
	normal = mul(normal, matNormal);//nomal��matW��������

	float4 light = float4(0,1,-1,0);//���̐i�ޕ����Ƃ͋t�̃x�N�g��
	light = normalize(light);//���K��
	outData.color = clamp(dot(normal, light), 0, 1);//dot�͓��ς����߂��������������A��O�����̊Ԃ̐��ɂ���


	//�܂Ƃ߂ďo��

	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	if (isTexture) {
	float4 diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color;//�g�U���ˌ�
	float4 ambient = g_texture.Sample(g_sampler, inData.uv) * float4(0.4, 0.4, 0.4, 1);//����
	return diffuse + ambient;
	}
    else
	{
		return diffuseColor * inData.color;
    }
}

//�֊s�\���p���_�V�F�[�_�[
float4 VS_Outline(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) : SV_POSITION
{
	pos = mul(pos, matWVP);
	return pos;
}

//�֊s�\���p�̃s�N�Z���V�F�[�_�[
float4 PS_Outline(VS_OUT inData) : SV_Target
{
	return float4 (0,0,0,1);
}
#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include"Macro.h"

//�����J
#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

class Texture;//�|�C���^�����ł���΂��ꂾ���ł��� �O���錾

class Fbx
{
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;   //Texture�^�̃|�C���^
		XMFLOAT4	diffuse; //�}�e���A���@���̃J���[
	};

	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;       // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
		XMMATRIX	matNormal;    //�@��
		XMFLOAT4    diffuseColor; //�}�e���A���̐F
		int		isTexture;		  // �e�N�X�`���\���Ă��邩�ǂ���
	};
	//���_
	struct VERTEX
	{
		XMVECTOR position; //�|�W�V����
		XMVECTOR uv;       //�e�N�X�`���̍��W
		XMVECTOR normal;   //�@��
	};

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_; //MATERIAL�^�̃|�C���^
	
public:
	//�R���X�g���N�^
	Fbx();                
	//���[�h
	//�����@�������t�@�C���̖��O
	HRESULT Load(std::string fileName);
	//�`��
	//�����@������Transform�^�̃|�C���^
	void    Draw(Transform& transform);
	//���
	void    Release();                 
private:
	//���_�o�b�t�@����
	//�����@������FbxMesh�^�̃|�C���^
	HRESULT InitVertex(fbxsdk::FbxMesh* pMesh);
	//�C���f�b�N�X�o�b�t�@����
	//�����@������FbxMesh�^�̃|�C���^
	HRESULT InitIndex(fbxsdk::FbxMesh* pMesh);
	//�R���X�^���g�o�b�t�@����
	HRESULT IntConstantBuffer();
	//�}�e���A������
	//�����@������FbxMesh�^�̃|�C���^
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
	
	
};

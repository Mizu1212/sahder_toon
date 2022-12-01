#include "Fbx.h"
#include"Direct3D.h"
#include "Camera.h"
#include"Texture.h"

//�R���X�g���N�^
Fbx::Fbx()
{
    vertexCount_ = 0;
    polygonCount_ = 0;
	materialCount_ = 0;
    pVertexBuffer_ = nullptr;
    pIndexBuffer_ = nullptr;
    pConstantBuffer_ = nullptr;
	pMaterialList_ = nullptr;
}

//���[�h
HRESULT Fbx::Load(std::string fileName)
{
	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* pRootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = pRootNode->GetChild(0);
	FbxMesh* pMesh = pNode->GetMesh();

	//�e���̌����擾

	vertexCount_ = pMesh->GetControlPointsCount();	//���_�̐�
	polygonCount_ = pMesh->GetPolygonCount();	//�|���S���̐�
	materialCount_ = pNode->GetMaterialCount(); //�}�e���A���̌�


	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

		//������fileName����f�B���N�g���������擾
		char dir[MAX_PATH];
		_splitpath_s(fileName.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
		sprintf_s(dir,dir);

		//�J�����g�f�B���N�g���ύX
		SetCurrentDirectory(dir);



	InitVertex(pMesh);		//���_�o�b�t�@����
	InitIndex(pMesh);		//�C���f�b�N�X�o�b�t�@����
	IntConstantBuffer();	//�R���X�^���g�o�b�t�@����
	InitMaterial(pNode);    //�}�e���A������

	//�I�������߂�
	SetCurrentDirectory(defaultCurrentDir);

	//�}�l�[�W�����
	pFbxManager->Destroy();


	pTexToon_ = new Texture;
	pTexToon_->Load(L"Assets\\����.png");


    return S_OK;
}

//���_�o�b�t�@����
HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* pMesh)
{
	//���_��������z��
	VERTEX* vertices = new VERTEX[vertexCount_];

	//�S�|���S��
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = pMesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = pMesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//���_��UV
			FbxLayerElementUV* pUV = pMesh->GetLayer(0)->GetUVs();
			int uvIndex = pMesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//���_�̖@��
			FbxVector4 Normal;
			pMesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}


	// ���_�o�b�t�@�쐬
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

//�C���f�b�N�X�o�b�t�@����
HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* pMesh)
{

	pIndexBuffer_ = new ID3D11Buffer* [materialCount_];
	int* index = new int[polygonCount_ * 3];

	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;

		//�S�|���S��
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = pMesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = pMesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}
		//�C���f�b�N�X�o�b�t�@�쐬
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
			return hr;
		}
	}
	return S_OK;
}

//�R���X�^���g�o�b�t�@����
HRESULT Fbx::IntConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	// �R���X�^���g�o�b�t�@�쐬
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}
//�}�e���A������
HRESULT Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
		FbxDouble3 diffuse = pMaterial->Diffuse;
		FbxDouble3 ambient = pMaterial->Ambient;
		FbxDouble3 specular = FbxDouble3(0,0,0);
		FbxDouble shiness = 1;
		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			 specular = pMaterial->Specular;
			 shiness = pMaterial->Shininess;
			 
		}
		
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterialList_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
		pMaterialList_[i].shiness = (float)shiness;
		
		//i�Ԗڂ̃}�e���A�������擾
		//FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		//�e�N�X�`�����
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//�e�N�X�`���̐�
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//�e�N�X�`������
		if (fileTextureCount > 0)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//�t�@�C����+�g�������ɂ���
			char name[_MAX_FNAME];	//�t�@�C����
			char ext[_MAX_EXT];	//�g���q
			_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
			sprintf_s(name, "%s%s", name, ext);


			//�t�@�C������e�N�X�`���쐬
			HRESULT hr;
			pMaterialList_[i].pTexture = new Texture;
			wchar_t wtext[FILENAME_MAX];
			size_t strlen(
				const char* textureFilePath //������̒��������߂�
			);
			size_t ret;
			mbstowcs_s(&ret, wtext, name, strlen(textureFilePath));//�}���`�o�C�g���烏�C�h�ύX

			hr = pMaterialList_[i].pTexture->Load(wtext);
			if (FAILED(hr))
			{
				MessageBox(NULL, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
				return hr;
			}
			
		}

		//�e�N�X�`������
		else
		{
			pMaterialList_[i].pTexture = nullptr;
			
			//�}�e���A���̐F
			//FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			//FbxDouble3  diffuse = pMaterial->Diffuse;
			//pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		}
	}
}
//�`��
void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_OUTLINE);
	
	for (int j = 0; j < 2; j++)
	{
		//���_�o�b�t�@
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
		for (int i = 0; i < materialCount_; i++)
		{

			// �C���f�b�N�X�o�b�t�@�[���Z�b�g
			stride = sizeof(int);
			offset = 0;
			Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

			//�R���X�^���g�o�b�t�@
			Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
			Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p


			CONSTANT_BUFFER cb;
			//cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
			//cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());//�e�̕t�����@�@��
			//cb.isTexture = pMaterialList_[i].pTexture != nullptr;
			cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
			cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
			cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
			XMStoreFloat4(&(cb.camPos), Camera::GetPosition());
			cb.diffusecolor = pMaterialList_[i].diffuse;
			cb.ambientcolor = pMaterialList_[i].ambient;
			cb.specularcolor = pMaterialList_[i].specular;
			cb.shiness = pMaterialList_[i].shiness;

			if (pMaterialList_[i].pTexture == nullptr)
			{
				cb.isTexture = false;
			}
			else
			{
				cb.isTexture = true;
			}
			if (pMaterialList_[i].pTexture) {

				ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
				Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
				ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
				Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

			}
			ID3D11ShaderResourceView* pSRVToon = pTexToon_->GetSRV();
			Direct3D::pContext->PSSetShaderResources(1, 1, &pSRVToon);

			//cb.diffuseColor = pMaterialList_[i].diffuse;
			D3D11_MAPPED_SUBRESOURCE pdata;
			Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
			memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

			Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ
			//�`��
			Direct3D::pContext->DrawIndexed(polygonCount_ * 3, 0, 0);
		}

		Direct3D::SetShader(SHADER_TOON);

	}

	
}
//���
void Fbx::Release()
{
	pTexToon_->Release();
	SAFE_DELETE(pTexToon_);
    SAFE_RELEASE(pVertexBuffer_);
	for (int i = 0; i < materialCount_; i++)
	{
		SAFE_RELEASE(pIndexBuffer_[i]);
    }
	SAFE_DELETE_ARRAY(pIndexBuffer_);
    SAFE_RELEASE(pConstantBuffer_);
}

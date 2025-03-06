#include "FbxLoader.h"

FbxLoader::FbxLoader()
{

}

FbxLoader::~FbxLoader()
{
	
}

bool FbxLoader::LoadFile(ID3D11Device* pDevice, HWND hwnd, std::string fileDirectory)
{
	bool result;

	FbxIOSettings* pIos;
	FbxImporter* pImporter;
	FbxScene* pScene;

	//FBX �Ŵ��� ����
	FbxManager* pManager = FbxManager::Create();

	//���� IO ����� ����(IOSROOT �⺻ ����)
	pIos = FbxIOSettings::Create(pManager, IOSROOT);

	//���� ����
	pManager->SetIOSettings(pIos);

	//FileName�� �ι�° �Ű������� �־� �� �̸��� �����ϴµ� ���� ������ ����
	//Scene ����
	pScene = FbxScene::Create(pManager, "");

	//�ҷ��� �����Ϳ� ���� �� �� �ִ� �������̽��� ����
	pImporter = FbxImporter::Create(pManager, "");

	//������ ����� �ε尡 �ƴ��� Ȯ��
	result = pImporter->Initialize(fileDirectory.c_str(), -1, pManager->GetIOSettings());
	if (!result)
	{
		MessageBox(hwnd, L"FbxImport Error", L"Error", MB_OK);
		std::string str = pImporter->GetStatus().GetErrorString();
		std::wstring w_trans = std::wstring(str.begin(), str.end());
		MessageBox(hwnd, w_trans.c_str(), L"Error", MB_OK);

		return false;
	}

	m_originPath = fileDirectory;

	//�ε�� ������ �����͸� Scene�� ��� Importer �ڿ� ��ȯ
	pImporter->Import(pScene);
	pImporter->Destroy();
	pImporter = nullptr;

	//���� ������ ���°� �簢������ �Ǿ� �ֱ⵵ �Ͽ� �ϴ� �ﰢ�� ���������� ��ȯ��
	FbxGeometryConverter converter(pManager);
	converter.Triangulate(pScene, true);//true�� ���� ������ ���� 

	//�� ������ ��� Ž��
	processNode(pScene->GetRootNode(), pDevice);

	//�޽����� ������ �ε��� ���۸� ����
	for (Mesh* m : m_meshes)
	{
		result = m->InitializeBuffer(pDevice);
		if (!result)
		{
			MessageBox(hwnd, L"Vertex Buffer Error", L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

void FbxLoader::processNode(FbxNode* pNode, ID3D11Device* pDevice)
{
	//����� �Ӽ��� ������
	FbxNodeAttribute* nodeAttribute = pNode->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
		//��� Ÿ���� �޽��� ���̷����� ��쿡�� �۾�
		switch (nodeAttribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			processMesh(pNode, pDevice);
			break;
		case FbxNodeAttribute::eSkeleton:
			break;
		}
	}

	//�ڽ� ��尡 �ִ��� Ȯ���ϸ� Ž��
	if (pNode->GetChildCount() > 0)
	{
		for (int i = 0; i < pNode->GetChildCount(); i++)
		{
			processNode(pNode->GetChild(i), pDevice);
		}
	}

	return;
}

void FbxLoader::processMesh(FbxNode* pNode, ID3D11Device* pDevice)
{
	bool result;

	FbxMesh* fm = pNode->GetMesh();

	//�޽� ����
	Mesh* pMesh = new Mesh;

	//����, �ε��� ����
	pMesh->InitializeMesh(fm);

	//�ؽ�ó�� �ִ� ��� ���� ��� ã��
	int materialCount = pNode->GetMaterialCount();
	if (materialCount > 0)
	{
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(0);
		if (!pMaterial)
		{
			return;
		}

		GetTextureFromMaterial(pMaterial, pDevice, pMesh);
	}

	m_meshes.push_back(pMesh);

	return;
}

//Material�� ���� �ؽ�ó ���� ��θ� �˾Ƴ��� �ش� ��θ� ���� �ؽ�ó�� �ε��ϴ� �Լ�
bool FbxLoader::GetTextureFromMaterial(FbxSurfaceMaterial* pMaterial, ID3D11Device* pDevice, Mesh* pMesh)
{
	HRESULT result;

	//Diffuse �Ӽ��� ������ 
	FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (!prop.IsValid())
	{
		return false;
	}

	//�ؽ�ó ���� Ȯ��
	int textureCount = prop.GetSrcObjectCount<FbxTexture>();
	if (!textureCount)
	{
		return false;
	}

	//�ؽ�ó ���� ������ ������
	FbxString fbxstr;
	FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>();
	if (!pTexture) {

		return false;
	}

	//���� ��ġ �˾Ƴ���
	fbxstr = pTexture->GetRelativeFileName();

	//�� ���� ��ġ�� �ؽ�ó ��� ��ġ�� ��ħ
	std::string tmpstr = m_originPath.substr(m_originPath.find_last_of("\\"));
	tmpstr.append("\\");
	tmpstr.append(fbxstr.Buffer());
	
	//���̵� ���ڷ� ��ȯ
	std::wstring wideStr(tmpstr.begin(), tmpstr.end());

	//�ؽ�ó ��� ����
	pMesh->SetTexture(pDevice, wideStr.c_str());

	//���� �ؽ�ó�� ��� ����
	m_prevPath = wideStr;
	pTexture->Destroy();

	return true;
}

void FbxLoader::Render(ID3D11DeviceContext* pDeviceContext)
{
	//���� �޽� �ϳ��� ������ ��
	for (Mesh* m : m_meshes)
	{
		m->Render(pDeviceContext);
	}

	return;
}

void FbxLoader::Shutdown()
{
	//�޽� ����
	for (Mesh* m : m_meshes)
	{
		delete m;
		m = nullptr;
	}

	//�迭 �ʱ�ȭ
	m_meshes.clear();

	return;
}


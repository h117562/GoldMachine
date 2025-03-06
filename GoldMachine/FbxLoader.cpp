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

	//FBX 매니저 생성
	FbxManager* pManager = FbxManager::Create();

	//파일 IO 입출력 설정(IOSROOT 기본 설정)
	pIos = FbxIOSettings::Create(pManager, IOSROOT);

	//설정 적용
	pManager->SetIOSettings(pIos);

	//FileName을 두번째 매개변수에 넣어 씬 이름을 저장하는데 딱히 쓸데는 없다
	//Scene 생성
	pScene = FbxScene::Create(pManager, "");

	//불러온 데이터에 접근 할 수 있는 인터페이스를 제공
	pImporter = FbxImporter::Create(pManager, "");

	//파일이 제대로 로드가 됐는지 확인
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

	//로드된 파일의 데이터를 Scene에 담고 Importer 자원 반환
	pImporter->Import(pScene);
	pImporter->Destroy();
	pImporter = nullptr;

	//보통 폴리곤 형태가 사각형으로 되어 있기도 하여 일단 삼각형 폴리곤으로 변환함
	FbxGeometryConverter converter(pManager);
	converter.Triangulate(pScene, true);//true시 원본 데이터 유지 

	//모델 파일의 노드 탐색
	processNode(pScene->GetRootNode(), pDevice);

	//메쉬마다 정점과 인덱스 버퍼를 생성
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
	//노드의 속성을 가져옴
	FbxNodeAttribute* nodeAttribute = pNode->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
		//노드 타입이 메쉬나 스켈레톤일 경우에만 작업
		switch (nodeAttribute->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			processMesh(pNode, pDevice);
			break;
		case FbxNodeAttribute::eSkeleton:
			break;
		}
	}

	//자식 노드가 있는지 확인하며 탐색
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

	//메쉬 생성
	Mesh* pMesh = new Mesh;

	//정점, 인덱스 저장
	pMesh->InitializeMesh(fm);

	//텍스처가 있는 경우 파일 경로 찾기
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

//Material을 통해 텍스처 파일 경로를 알아내고 해당 경로를 통해 텍스처를 로드하는 함수
bool FbxLoader::GetTextureFromMaterial(FbxSurfaceMaterial* pMaterial, ID3D11Device* pDevice, Mesh* pMesh)
{
	HRESULT result;

	//Diffuse 속성을 가져옴 
	FbxProperty prop = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (!prop.IsValid())
	{
		return false;
	}

	//텍스처 보유 확인
	int textureCount = prop.GetSrcObjectCount<FbxTexture>();
	if (!textureCount)
	{
		return false;
	}

	//텍스처 파일 정보를 가져옴
	FbxString fbxstr;
	FbxFileTexture* pTexture = prop.GetSrcObject<FbxFileTexture>();
	if (!pTexture) {

		return false;
	}

	//파일 위치 알아내기
	fbxstr = pTexture->GetRelativeFileName();

	//모델 파일 위치와 텍스처 상대 위치를 합침
	std::string tmpstr = m_originPath.substr(m_originPath.find_last_of("\\"));
	tmpstr.append("\\");
	tmpstr.append(fbxstr.Buffer());
	
	//와이드 문자로 변환
	std::wstring wideStr(tmpstr.begin(), tmpstr.end());

	//텍스처 경로 전달
	pMesh->SetTexture(pDevice, wideStr.c_str());

	//이전 텍스처의 경로 저장
	m_prevPath = wideStr;
	pTexture->Destroy();

	return true;
}

void FbxLoader::Render(ID3D11DeviceContext* pDeviceContext)
{
	//모델의 메쉬 하나씩 렌더링 함
	for (Mesh* m : m_meshes)
	{
		m->Render(pDeviceContext);
	}

	return;
}

void FbxLoader::Shutdown()
{
	//메쉬 해제
	for (Mesh* m : m_meshes)
	{
		delete m;
		m = nullptr;
	}

	//배열 초기화
	m_meshes.clear();

	return;
}


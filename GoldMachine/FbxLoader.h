#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

#pragma comment(lib, "libfbxsdk-mt.lib")

#include <fbxsdk.h>
#include <string>

#include "Mesh.h"

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	bool LoadFile(ID3D11Device*, HWND, std::string FileDirectoryPath);
	void Shutdown();
	void Render(ID3D11DeviceContext* pDeviceContext);
private:
	void processNode(FbxNode*, ID3D11Device* pDevice);
	void processMesh(FbxNode*, ID3D11Device* pDevice);
	bool GetTextureFromMaterial(FbxSurfaceMaterial*, ID3D11Device* pDevice, Mesh* mesh);

private:
	std::vector<Mesh*> m_meshes;
	std::string m_originPath;
	std::wstring m_prevPath;
};

#endif
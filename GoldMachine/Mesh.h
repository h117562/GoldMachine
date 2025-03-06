#ifndef _MESH_H_
#define _MESH_H_

#pragma comment(lib, "libfbxsdk-mt.lib")

#include <fbxsdk.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureResource.h"
#include "GraphicStructures.h"

class Mesh
{
public:
	Mesh()
	{
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;

		m_vertices = nullptr;
		m_indices = nullptr;
		m_texture = nullptr;

		m_vCounts = 0;
		m_iCounts = 0;
	}

	~Mesh()
	{
		if (m_texture)
		{
			delete m_texture;
			m_texture = nullptr;
		}

		if (m_vertices)
		{
			delete[] m_vertices;
			m_vertices = nullptr;
		}

		if (m_indices)
		{
			delete[] m_indices;
			m_indices = nullptr;
		}

		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = nullptr;
		}

		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = nullptr;
		}
	}

	bool InitializeMesh(FbxMesh* pMesh)
	{

		if (pMesh->IsTriangleMesh())
		{
			//메쉬의 모든 정점을 가져옴 
			FbxVector4* fv4 = pMesh->GetControlPoints();

			//UVSet 이름을 가져옴
			FbxStringList uvNames;
			pMesh->GetUVSetNames(uvNames);

			//폴리곤 개수를 가져옴
			const int polygonCount = pMesh->GetPolygonCount();
			const int vertexCount = pMesh->GetPolygonVertexCount();//정점 개수 polygonCount*3

			//배열 생성
			m_vertices = new VertexUV[vertexCount];

			//정점 위치 저장
			for (int p = 0; p < polygonCount; p++)//폴리곤 번호
			{
				for (int v = 0; v < 3; v++)//정점 번호
				{
					bool unmappedUV;//텍스쳐 매핑 여부
					
					//인덱스를 얻음
					UINT index = pMesh->GetPolygonVertex(p, v);

					//정점 좌표를 저장
					m_vertices[p + v].position = DirectX::XMVectorSet(
						static_cast<float>(fv4[index].mData[0]),
						static_cast<float>(fv4[index].mData[1]),
						static_cast<float>(fv4[index].mData[2]), 
						0);

					//텍스처 좌표를 저장
					FbxVector2 fv2;
					pMesh->GetPolygonVertexUV(p, v, uvNames[0], fv2, unmappedUV);

					if (unmappedUV)
					{
						//TODO
					}

					m_vertices[p + v].textureCoord = DirectX::XMFLOAT2(
						static_cast<float>(fv2.mData[0]),
						1.0f - static_cast<float>(fv2.mData[1])
					);
				}
			}

			//인덱스 순서대로 나열
			m_indices = new UINT[vertexCount];

			for (int i = 0; i < vertexCount; i++)
			{
				m_indices[i] = i;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool InitializeBuffer(ID3D11Device* pDevice)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexUV) * m_vCounts;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_vertices;

		hr = pDevice->CreateBuffer(&vertexBufferDesc, &initData, &m_vertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * m_iCounts;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		initData.pSysMem = m_indices;

		hr = pDevice->CreateBuffer(&indexBufferDesc, &initData, &m_indexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	void Render(ID3D11DeviceContext* pDeviceContext) 
	{
		UINT stride = sizeof(VertexUV);
		UINT offset = 0;

		ID3D11ShaderResourceView* resourceView = m_texture->GetResourceView();

		pDeviceContext->PSSetShaderResources(0, 1, &resourceView);
		pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pDeviceContext->DrawIndexed(m_iCounts, 0, 0);

		return;
	}

	bool SetTexture(ID3D11Device* pDevice, const wchar_t* filePath)
	{
		HRESULT result;

		m_texture = new TextureResource;

		result = m_texture->Initialize(pDevice, filePath, -1);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	VertexUV* m_vertices;
	UINT* m_indices;
	TextureResource* m_texture;
	UINT m_vCounts, m_iCounts;
};

#endif
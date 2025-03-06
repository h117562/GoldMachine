#ifndef _TEXTURE_RESOURCE_H_
#define _TEXTURE_RESOURCE_H_

#pragma comment(lib, "../data/lib/DirectXTex.lib")

#include "../data/lib/DirectXTex.h"

class TextureResource
{
public:
	TextureResource()
	{
		m_resource = nullptr;
		m_resourceView = nullptr;
	}

	~TextureResource()
	{
		if (m_resource)
		{
			m_resource->Release();
			m_resource = nullptr;
		}

		if (m_resourceView)
		{
			m_resourceView->Release();
			m_resourceView = nullptr;
		}
	}

	HRESULT Initialize(ID3D11Device* pDevice, const wchar_t* filePath, const UINT miplevels)
	{
		HRESULT result;

		DirectX::ScratchImage sImage;
		
		//�̹��� �����͸� �ε�
		result = DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS_NONE, nullptr, sImage);
		if (FAILED(result))
		{
			return result;
		}

		//���� �̹����� �����ͷ� �ؽ��� ���ҽ� ����
		result = DirectX::CreateTextureEx(pDevice,
			sImage.GetImages(),
			sImage.GetImageCount(),
			sImage.GetMetadata(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0,
			0,
			DirectX::CREATETEX_DEFAULT,
			&m_resource);

		if (FAILED(result))
		{
			return result;
		}

		//���̴� ���ҽ� �� ����
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderResourceViewDesc.Format = sImage.GetMetadata().format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;//���ҽ� �� ����
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;//���� �ڼ��� Mipmap�� �ε���
		shaderResourceViewDesc.Texture2D.MipLevels = miplevels;//�ִ� Mipmap ���� 

		//���̴� ���ҽ� �� ����
		result = pDevice->CreateShaderResourceView(m_resource, &shaderResourceViewDesc, &m_resourceView);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	ID3D11Resource* GetResource()
	{
		return m_resource;
	};

	ID3D11ShaderResourceView* GetResourceView()
	{
		return m_resourceView;
	};

private:
	ID3D11Resource* m_resource;
	ID3D11ShaderResourceView* m_resourceView;
};

#endif
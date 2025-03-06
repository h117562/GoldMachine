#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Texture2DShader.h"

class ShaderManager
{
public:
	ShaderManager()
	{
		m_texture2DShader = nullptr;
	};

	~ShaderManager()
	{
		if (m_texture2DShader)
		{
			m_texture2DShader->Shutdown();
			m_texture2DShader = nullptr;
		}
	};

	bool Initialize(ID3D11Device* pDevice, HWND hwnd)
	{
		bool result = false;

		if (m_texture2DShader == nullptr)
		{
			m_texture2DShader = new Texture2DShader;
			if (!m_texture2DShader)
			{
				MessageBox(hwnd, L"Could not initialize the Texture2DShader.", L"Error", MB_OK);
				return false;
			}

			result = m_texture2DShader->Initialize(pDevice, hwnd);
			if (!result)
			{
				MessageBox(hwnd, L"Could not initialize the Texture2DShader.", L"Error", MB_OK);
				return false;
			}
		}
	};

	Texture2DShader* GetTexture2DShader()
	{
		return m_texture2DShader;
	}

private:
	Texture2DShader* m_texture2DShader;
};

#endif
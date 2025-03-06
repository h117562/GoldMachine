#ifndef _TEXTURE2D_SHADER_H_
#define _TEXTURE2D_SHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "GraphicStructures.h"

using namespace DirectX;

class Texture2DShader
{
public:
	Texture2DShader();
	~Texture2DShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix);

private:
	void OutputShaderErrorMessage(ID3DBlob*, HWND);
	bool UpdateShaderBuffers(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_samplerState;
};


#endif
#ifndef _RECT_TRANSFORM_
#define _RECT_TRANSFORM_

#include <DirectXMath.h>
#include <d2d1.h>

#include "Global.h"

enum ALIGNMENT//정렬 플래그
{
	ALIGNMENT_CENTER,//정중앙
	ALIGNMENT_LEFT,//좌측 중앙
	ALIGNMENT_RIGHT,//우측 중앙
	ALIGNMENT_TOP,//상단 중앙
	ALIGNMENT_BOTTOM,//하단 중앙
	ALIGNMENT_LEFT_TOP,//좌상단
	ALIGNMENT_LEFT_BOTTOM,//좌하단
	ALIGNMENT_RIGHT_TOP,//우상단
	ALIGNMENT_RIGHT_BOTTOM,//우하단
};

class RectTransform//요소의 위치 수정 및 정렬 기능을 제공하는 추상 클래스
{
protected:
	RectTransform()
	{
		//기본 설정 데이터
		m_localPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_worldPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		m_align = ALIGNMENT_CENTER;
		m_worldMatrix = DirectX::XMMatrixIdentity();
	}

	~RectTransform()
	{
	}

public:
	//상대 위치 설정
	void SetLocalPosition(const float& x, const float& y, const float& z)
	{
		m_localPosition = DirectX::XMFLOAT3(x, y, z);
	}

	//크기 설정
	void SetScale(const float& x, const float& y, const float& z)
	{
		m_scale = DirectX::XMFLOAT3(x, y, z);
	}

	//정렬 설정
	void SetAlign(const unsigned int& flag)
	{
		m_align = flag;
	}

	void SetTransform(XMFLOAT3 position, XMFLOAT3 scale, UINT align)
	{
		m_localPosition = position;
		m_scale = scale;
		m_align = align;

		UpdateWorldMatrix();
	}

	void UpdateTransform()
	{
		UpdateWorldMatrix();
	}

	//월드 행렬 가져오기
	DirectX::XMMATRIX GetWorldMatrix()
	{
		return m_worldMatrix;
	}

	//상대 좌표 얻기
	DirectX::XMFLOAT3 GetLocalPosition()
	{
		return m_localPosition;
	}

	//절대 좌표 얻기
	DirectX::XMFLOAT3 GetWorldPosition()
	{
		return m_worldPosition;
	}

	//크기 얻기
	DirectX::XMFLOAT3 GetScale()
	{
		return m_scale;
	}

	//정렬 얻기
	unsigned int GetAlign()
	{
		return m_align;
	}

private:
	DirectX::XMMATRIX Alignment(const unsigned int& screenWidth, const unsigned int& screenHeight, const unsigned int& flag)
	{
		DirectX::XMMATRIX mat;
		float x, y;

		switch (flag)
		{
		case ALIGNMENT_CENTER://정중앙
			//상대좌표가 곧 절대좌표
			m_worldPosition = DirectX::XMFLOAT3(m_localPosition.x / m_scale.x, m_localPosition.y / m_scale.y, m_localPosition.z / m_scale.z);
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT://좌측 중앙
			x = CalculateElementPosition(screenWidth, m_scale.x);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT://우측 중앙
			x = CalculateElementPosition(screenWidth, m_scale.x);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_TOP://상단 중앙
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3((m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_BOTTOM://하단 중앙
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3((m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT_TOP://좌상단
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT_BOTTOM://좌하단
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT_TOP://우상단
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT_BOTTOM://우하단
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		default://ALIGNMENT_CENTER
			//상대좌표가 곧 절대좌표
			m_worldPosition = DirectX::XMFLOAT3(m_localPosition.x / m_scale.x, m_localPosition.y / m_scale.y, m_localPosition.z / m_scale.z);
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
		}

		return mat;
	}

	float CalculateElementPosition(const unsigned int& resolution, const float& scale)
	{
		if (scale < 1.0f)
		{
			return static_cast<float>(resolution) / 2.0f - 0.5f;
		}

		return static_cast<float>(resolution) / (scale * 2.0f) - 0.5f;
	}

	//월드 행렬 업데이트
	void UpdateWorldMatrix()
	{
		DirectX::XMMATRIX pos, scale;

		m_worldMatrix = DirectX::XMMatrixIdentity();

		pos = Alignment(SCREEN_WIDTH, SCREEN_HEIGHT, m_align);
		scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, pos);
		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, scale);

		return;
	}

private:
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_localPosition;
	DirectX::XMFLOAT3 m_worldPosition;
	DirectX::XMFLOAT3 m_scale;
	unsigned int m_align;
};

#endif
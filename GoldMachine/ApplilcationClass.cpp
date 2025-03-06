#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_TextClass = 0;
	m_ShaderManager = 0;
}

ApplicationClass::~ApplicationClass()
{
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

bool ApplicationClass::Initialize(HWND hwnd)
{
	bool result;

	m_Direct3D = new DirectXClass;
	if (!m_Direct3D)
	{
		return false;
	}

	//DirectX 초기화
	result = m_Direct3D->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_ShaderManager = new ShaderManager;
	if (!m_ShaderManager)
	{
		return false;
	}

	//쉐이더 매니저 초기화
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ShaderManager.", L"Error", MB_OK);
		return false;
	}

	m_TextClass = new TextClass;
	if (!m_TextClass)
	{
		return false;
	}

	//TextClass 초기화
	result = m_TextClass->Initialize(m_Direct3D->GetSwapChain());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextClass.", L"Error", MB_OK);
		return false;
	}

	return result;
}

void ApplicationClass::Shutdown()
{

	if (m_ShaderManager)
	{
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	if (m_TextClass)
	{
		m_TextClass->Shutdown();
		delete m_TextClass;
		m_TextClass = nullptr;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	return;
}

bool ApplicationClass::Frame(HWND hwnd, FrameTimer* pFrameTimer)
{
	bool result;

	//3D RenderTarget 초기화(특정 컬러로)
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.2f, 1.0f);

	//2D RenderTarget 초기화
	m_TextClass->BeginDraw();

	//렌더링
	Render(hwnd);

	m_TextClass->EndDraw();
	m_Direct3D->EndScene();

	return true;
}

void ApplicationClass::HandleInput(FrameTimer* pFrameTimer)
{
	bool state;

	return;
}


void ApplicationClass::Render(HWND hwnd)
{
	
	return;
}
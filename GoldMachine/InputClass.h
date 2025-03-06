#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define KEYDOWN(keyState, keyCode) (keyState[keyCode] & 0x80) 

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <memory.h>
#include <wchar.h>

class InputClass
{
public:
	static InputClass& GetInstance()
	{
		static InputClass instance;

		return instance;
	}

	//복사 생성자랑 대입 연산 제거
	InputClass(const InputClass&) = delete;
	void operator=(const InputClass&) = delete;

private:
	InputClass();
	~InputClass();

public:
	bool Initialize(HINSTANCE, HWND);
	bool Frame();

	bool GetKeyPressed(const unsigned char keyCode);
	bool GetKeyPressedAndRelease(const unsigned char keyCode);
	bool GetKeyReleasedAndPress(const unsigned char keyCode);

	bool GetLeftMouseButtonPressed();
	bool GetLeftMouseButtonReleased();
	bool GetRightMouseButtonPressed();
	bool GetRightMouseButtonReleased();

	bool GetLeftMouseButtonPressedAndReleased();
	bool GetLeftMouseButtonReleasedAndPressed();

	void GetMousePosition(long&, long&);
	void GetNormalizedMousePosition(float&, float&);
	void GetMouseDelta(float&, float&);

private:
	bool ReadKeyboard();
	bool ReadMouse();

private:
	HWND m_hwnd;
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_keyboardDevice, m_mouseDevice;
	DIMOUSESTATE2 m_mouseState;
	unsigned char m_currKeyState[256];//현재 키보드 상태
	unsigned char m_prevKeyState[256];//이전 키보드 상태
	unsigned char m_prevMouseState[8];//이전 마우스 상태
	float m_sensitivity;//마우스 민감도
};

#endif

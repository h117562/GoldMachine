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

	//���� �����ڶ� ���� ���� ����
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
	unsigned char m_currKeyState[256];//���� Ű���� ����
	unsigned char m_prevKeyState[256];//���� Ű���� ����
	unsigned char m_prevMouseState[8];//���� ���콺 ����
	float m_sensitivity;//���콺 �ΰ���
};

#endif

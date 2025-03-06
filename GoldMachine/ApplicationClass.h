#ifndef _APPLICATION_CLASS_H_
#define _APPLICATION_CLASS_H_

#include "DirectXClass.h"
#include "EventClass.h"
#include "InputClass.h"
#include "ShaderManager.h"
#include "TextClass.h"
#include "FrameTimer.h"

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass();
	ApplicationClass(const ApplicationClass&);

	bool Initialize(HWND);
	void Shutdown();
	bool Frame(HWND, FrameTimer*);

private:
	void Render(HWND);
	void HandleInput(FrameTimer*);

private:
	DirectXClass* m_Direct3D;
	TextClass* m_TextClass;
	ShaderManager* m_ShaderManager;
};
#endif
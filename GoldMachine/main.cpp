#include <Windows.h>

#include "Global.h"
#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdshow)
{
	bool result;

	SystemClass* systemClass;

	//SystemClass 생성
	systemClass = new SystemClass;
	if (!systemClass)
	{
		return 0;
	}

	//SystemClass 초기화
	result = systemClass->Initialize();
	if (result)
	{
		systemClass->Run();
	}

	//System Class 메모리 반환
	systemClass->Shutdown();
	delete systemClass;
	systemClass = 0;

	return 0;
}
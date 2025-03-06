#include <Windows.h>

#include "Global.h"
#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdshow)
{
	bool result;

	SystemClass* systemClass;

	//SystemClass ����
	systemClass = new SystemClass;
	if (!systemClass)
	{
		return 0;
	}

	//SystemClass �ʱ�ȭ
	result = systemClass->Initialize();
	if (result)
	{
		systemClass->Run();
	}

	//System Class �޸� ��ȯ
	systemClass->Shutdown();
	delete systemClass;
	systemClass = 0;

	return 0;
}
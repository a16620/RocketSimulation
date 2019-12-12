#include "MainFrame.h"

HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	hInst = hInstance;
	MainFrame mf;
	mf.Initialize(hInstance, hPrevInstance, lpszCmdParam, nCmdShow);
	mf.Run();
	return 0;
}

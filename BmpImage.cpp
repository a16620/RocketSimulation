#include "BmpImage.h"
#pragma comment(lib, "msimg32.lib")

extern HINSTANCE hInst;

CBitmap::CBitmap(const std::string & directory)
{
	if (directory.empty())
		throw std::runtime_error("CBitmap:Unavilable directory");
	memset(&m_BitmapInfo, 0, sizeof(m_BitmapInfo));
	m_BitmapHandle = (HBITMAP)LoadImageA(hInst, directory.c_str(), IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (m_BitmapHandle == nullptr)
		throw std::runtime_error("CBitmap:Load failed");

	GetObject(m_BitmapHandle, sizeof(m_BitmapInfo), &m_BitmapInfo);
	m_strPath = directory;
	m_dwTransparent = RGB(255, 255, 255);
}

CBitmap::~CBitmap()
{
	DeleteObject(m_BitmapHandle);
}

void CBitmap::DrawBitmap(const HDC & hdc, int x, int y, int width, int height) const
{
	DrawBitmapByCropping(hdc, x, y, width, height, 0, 0,
		m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight);
}

void CBitmap::DrawBitmapByCropping(const HDC & hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh) const
{
	HDC hMemDC = CreateCompatibleDC(nullptr);

	SelectObject(hMemDC, m_BitmapHandle);

	TransparentBlt(
		hdc, dx, dy,
		dw == 0 ? m_BitmapInfo.bmWidth : dw,
		dh == 0 ? m_BitmapInfo.bmHeight : dh,
		hMemDC, sx, sy, sw, sh, m_dwTransparent);

	DeleteDC(hMemDC);
}

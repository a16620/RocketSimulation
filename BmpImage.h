#pragma once
#include <Windows.h>
#include <string>

class CBitmap
{
private:
	DWORD m_dwTransparent;

	HBITMAP m_BitmapHandle;
	BITMAP m_BitmapInfo;
	std::string m_strPath;
public:
	CBitmap(const std::string& directory);
	~CBitmap();

	const HBITMAP& getHandle() const {
		return m_BitmapHandle;
	}

	const BITMAP getInfo() const {
		return m_BitmapInfo;
	}

	std::string getFileDirectory() const {
		return m_strPath;
	}

	void DrawBitmap(const HDC& hdc, int x, int y, int width, int height) const;
	void DrawBitmapByCropping(const HDC & hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh) const;

	DWORD getTransparentColor() {
		return m_dwTransparent;
	}
	void setTransparentColor(DWORD color) {
		m_dwTransparent = color;
	}
};


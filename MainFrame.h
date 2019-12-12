#pragma once
#include "CScene.h"
#include <map>
using namespace std;

enum PARAMTYPE {
	PARAM_NONE,
	PARAM_FLOAT,
	PARAM_INT
};
struct SCENEPARAMETER {
	PARAMTYPE type;
	union {
		_m_d_float float_data;
		_m_d_int int_data;
	};
};

class DoubleBuffer;

class MainFrame
{
private:
	HINSTANCE hInst;
	HWND hWnd;
	HDC hdc;
	DoubleBuffer* m_buffer;
	LPCSTR lpszClass = "Simulator";
	RECT rc{ 0,0, WND_SIZE_X, WND_SIZE_Y };
	map<string, CScene*> m_sceneMap;
	CScene* currentScene;
	map<string, SCENEPARAMETER> m_paramMap;

public:
	MainFrame();
	~MainFrame();

	void Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
	void Run();

	void Update(const _m_d_float& dt);
	void Render();

	void ChangeScene(const string& name);
	void RegisterScene(const string& name, CScene* scene);

	const SCENEPARAMETER getParam(const string& key) const;

	void putParam(const string& key, _m_d_int value);
	void putParam(const string& key, _m_d_float value);

private:
	static MainFrame* _Instance;
public:
	static MainFrame* getInstance() {
		return _Instance;
	}
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DoubleBuffer {
private:
	HDC& m_hdc;
	const RECT rc{ 0,0, WND_SIZE_X, WND_SIZE_Y };
	HDC m_memDC;
	HBITMAP m_bmp, m_oldBmp;
public:
	DoubleBuffer(HDC& hdc);
	~DoubleBuffer();
	void Swap();
	void Draw();
	const HDC& getDC() const {
		return m_memDC;
	}
};
#include "MainFrame.h"

MainFrame* MainFrame::_Instance = NULL;

MainFrame::MainFrame()
{
	currentScene = NULL;
	_Instance = this;
}

MainFrame::~MainFrame()
{
	for (auto it : m_sceneMap) {
		delete it.second;
	}
	m_sceneMap.clear();
	m_paramMap.clear();
	delete m_buffer;
	ReleaseDC(hWnd ,hdc);
}

Atom* BuildAtom(const Vector2& position, const Vector2& velocity, _m_d_float mass, _m_d_int radius, _m_d_float charge = 0) {
	Atom* at = new Atom(radius);
	at->position = position;
	at->velocity = velocity;
	at->charge = charge;
	at->mass = mass;
	return at;
}

void MainFrame::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//Init wnd
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WND_SIZE_X, WND_SIZE_Y,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hdc = GetDC(hWnd);
	m_buffer = new DoubleBuffer(hdc);
	RegisterScene("main", new SimulatingScene());
	RegisterScene("w", new SettingScene());
	ChangeScene("w");
}

void MainFrame::Run()
{
	MSG msg;
	DWORD time=GetTickCount();
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		DWORD ct = GetTickCount();
		if (ct > time)
		{
			const _m_d_float dt = (ct - time) *0.01f;// / 100.0f;
			time = ct + 50;
			Update(dt);
			Render();
		}
	}
}

void MainFrame::Update(const _m_d_float& dt)
{
	currentScene->Update(dt);
}

void MainFrame::Render()
{
	currentScene->Render(m_buffer->getDC());
	m_buffer->Draw();
}

void MainFrame::ChangeScene(const string& name)
{
	auto it = m_sceneMap.find(name);
	if (it == m_sceneMap.end())
		return;

	if (currentScene != nullptr)
	{
		currentScene->Release();
	}

	currentScene = it->second;
	currentScene->Initialize();
}

void MainFrame::RegisterScene(const string& name, CScene * scene)
{
	auto it = m_sceneMap.find(name);
	if (it != m_sceneMap.end())
		return;

	m_sceneMap.insert(make_pair(name, scene));
}

const SCENEPARAMETER MainFrame::getParam(const string & key) const
{
	auto it = m_paramMap.find(key);
	if (it == m_paramMap.end())
		return SCENEPARAMETER{ PARAMTYPE::PARAM_NONE };
	return it->second;
}

void MainFrame::putParam(const string & key, _m_d_int value)
{
	auto it = m_paramMap.find(key);
	if (it == m_paramMap.end()) {
		auto d = SCENEPARAMETER{ PARAMTYPE::PARAM_INT };
		d.int_data = value;
		m_paramMap.insert(make_pair(key, d));
	}
	else {
		auto d = SCENEPARAMETER{ PARAMTYPE::PARAM_INT };
		d.int_data = value;
		it->second = d;
	}
}

void MainFrame::putParam(const string & key, _m_d_float value)
{
	auto it = m_paramMap.find(key);
	if (it == m_paramMap.end()) {
		auto d = SCENEPARAMETER{ PARAMTYPE::PARAM_INT };
		d.float_data = value;
		m_paramMap.insert(make_pair(key, d));
	}
	else {
		auto d = SCENEPARAMETER{ PARAMTYPE::PARAM_INT };
		d.float_data = value;
		it->second = d;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

DoubleBuffer::DoubleBuffer(HDC& hdc) : m_hdc(hdc)
{
	m_memDC = CreateCompatibleDC(hdc);
	m_bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	m_oldBmp = (HBITMAP)SelectObject(m_memDC, m_bmp);
}

DoubleBuffer::~DoubleBuffer()
{
	DeleteDC(m_memDC);
	DeleteObject(m_bmp);
}

void DoubleBuffer::Swap()
{
	DeleteObject(SelectObject(m_memDC, m_oldBmp));
	DeleteDC(m_memDC);
	m_memDC = CreateCompatibleDC(m_hdc);
	m_bmp = CreateCompatibleBitmap(m_hdc, rc.right, rc.bottom);
	m_oldBmp = (HBITMAP)SelectObject(m_memDC, m_bmp);
	PatBlt(m_memDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);
}

void DoubleBuffer::Draw()
{
	BitBlt(m_hdc, rc.left, rc.top, rc.right, rc.bottom, m_memDC, 0, 0, SRCCOPY);
	Swap();
}

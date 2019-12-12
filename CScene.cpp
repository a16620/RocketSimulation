#include "CScene.h"
#include "MainFrame.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

void SimulatingScene::Initialize()
{
	//Init objs	
	//m_objList.push_back(BuildAtom(Vector2(350, 300), Vector2(_m_sqrt(20.f), 0), 4, 4));
	//m_objList.push_back(new KineticObject(Vector2(350, 350), 10));
	m_objList.push_back(new BackGroundImage());
	{
		Rocket* rk = new Rocket();
		rk->position = Vector2(100, 300);
		rk->mass = MainFrame::getInstance()->getParam("RM").float_data+ MainFrame::getInstance()->getParam("FM").float_data;
		rk->fuel = MainFrame::getInstance()->getParam("FM").float_data;
		rk->fuel_vel = MainFrame::getInstance()->getParam("FV").float_data;
		rk->dfuel = MainFrame::getInstance()->getParam("DM").float_data;
		m_objList.push_back(rk);
		m_objList.push_back(new CObjectInfoRenderer(rk, Vector2(300, 600)));
	}
	//m_adpList.push_back(new GravityMovement());
}

void SimulatingScene::Render(const HDC & hdc)
{
	for (auto obj : m_objList) {
		obj->Render(hdc);
	}
}

void SimulatingScene::Update(const _m_d_float & dt)
{
	for (auto adaptor : m_adpList) {
		adaptor->Perform(&m_objList, dt);
	}
	for (auto obj : m_objList) {
		obj->Update(dt);
	}
}

void SimulatingScene::Release()
{
	for (auto obj : m_objList) {
		delete obj;
	}
	for (auto obj : m_adpList) {
		delete obj;
	}
}

void SettingScene::ChangeValue(int i, _m_d_float val)
{
	if (valS[i] + val < 0)
		valS[i] = 0;
	else
		valS[i] += val;
	sprintf_s(tmpBuffer[i], 32, "%.2f", valS[i]);
	lenBuffer[i] = strlen(tmpBuffer[i]);
}

void SettingScene::Initialize()
{
	for (int i = 0; i < 4; i++) {
		ChangeValue(i, 0);
	}
}

void SettingScene::Render(const HDC & hdc)
{
	Ellipse(hdc, 293, 305 + ci * 25, 298, 310 + ci * 25);
	TextOutA(hdc, 300, 300, tmpBuffer[0], lenBuffer[0]);
	TextOutA(hdc, 350, 300, "Rocket Mass", 11);

	TextOutA(hdc, 300, 325, tmpBuffer[1], lenBuffer[1]);
	TextOutA(hdc, 350, 325, "Fuel Mass", 9);

	TextOutA(hdc, 300, 350, tmpBuffer[2], lenBuffer[2]);
	TextOutA(hdc, 350, 350, "Fuel V", 6);

	TextOutA(hdc, 300, 375, tmpBuffer[3], lenBuffer[3]);
	TextOutA(hdc, 350, 375, "Fuel dM", 7);
}

void SettingScene::Update(const _m_d_float & dt)
{
	if (GetAsyncKeyState(VK_DOWN) & 0x81) {
		if (ci < 3)
			ci++;
	}
	if (GetAsyncKeyState(VK_UP) & 0x81) {
		if (ci > 0)
			ci--;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		ChangeValue(ci, -dt);
		
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		ChangeValue(ci, dt);
	}

	if (GetAsyncKeyState(VK_RETURN)) {
		MainFrame::getInstance()->putParam("RM", valS[0]);
		MainFrame::getInstance()->putParam("FM", valS[1]);
		MainFrame::getInstance()->putParam("FV", valS[2]);
		MainFrame::getInstance()->putParam("DM", valS[3]);
		MainFrame::getInstance()->ChangeScene("main");
	}
}

void SettingScene::Release()
{
}

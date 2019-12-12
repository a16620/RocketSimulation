#pragma once
#include <vector>
#include "CObject.h"
#include "AdditionalCalculations.h"
using namespace std;

class CScene
{
public:
	virtual void Initialize() = 0;
	virtual void Render(const HDC& hdc) = 0;
	virtual void Update(const _m_d_float& dt) = 0;
	virtual void Release() = 0;

	CScene();
	virtual ~CScene();
};

class SimulatingScene : public CScene {
private:
	vector<CObject*> m_objList;
	vector<AdditionalCalculation*>  m_adpList;
public:
	void Initialize();
	void Render(const HDC& hdc);
	void Update(const _m_d_float& dt);
	void Release();
};

class SettingScene : public CScene {
private:
	_m_d_float valS[4] = { 0 };
	char tmpBuffer[4][32];
	int lenBuffer[4];
	int ci = 0;

	void ChangeValue(int i, _m_d_float val);
public:
	void Initialize();
	void Render(const HDC& hdc);
	void Update(const _m_d_float& dt);
	void Release();
};
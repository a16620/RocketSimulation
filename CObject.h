#pragma once
#include <Windows.h>
#include "Defines.h"
#include "Mathlib.h"
#include "BmpImage.h"

interface IRunnable {
public:
	virtual void Render(const HDC& hdc) = 0;
	virtual void Update(const _m_d_float& dt) = 0;
};
//void Render(const HDC& hdc) {} void Update(const _m_d_float& dt) {}

class Collider {
public:
	enum COLLIDER_TYPE {
		COLTYPE_DEFAULT,
		COLTYPE_ATOM,
		COLTYPE_IGNORE
	};
protected:
	COLLIDER_TYPE colType;
	
public:
	Collider() { colType = COLLIDER_TYPE::COLTYPE_DEFAULT; }
	Collider(COLLIDER_TYPE type);
	inline COLLIDER_TYPE getColType() const {
		return colType;
	}
	virtual bool CheckCollision(Collider* other, _m_d_float* overlapped) = 0;
};
//bool CheckCollision(Collider* other, _m_d_float* overlapped)

class CObject : public IRunnable, public Collider
{
public:
	_m_d_float mass;
	_m_d_float charge;
	Vector2 position;
	Vector2 velocity;

	CObject();
	virtual ~CObject() {}
};


class Atom : public CObject {
protected:
	_m_d_int radius;
public:
	Atom(_m_d_int _radius);
	void Render(const HDC& hdc);
	void Update(const _m_d_float& dt);
	bool CheckCollision(Collider* other, _m_d_float* overlapped);
};

class KineticObject : public Atom {
public:
	KineticObject(const Vector2& _position, _m_d_float _mass);
	void Update(const _m_d_float& dt) override {}
};

class Rocket : public CObject {
private:
	CBitmap* m_image[2];
public:
	_m_d_float fuel, dfuel, fuel_vel;
public:
	Rocket() {
		m_image[0] = new CBitmap("img_f.bmp");
		m_image[1] = new CBitmap("img_n.bmp");
		fuel = 10;
		dfuel = 1;
	}
	Rocket(_m_d_float mf, _m_d_float df) : fuel(mf), dfuel(df) {
		m_image[0] = new CBitmap("img_f.bmp");
		m_image[1] = new CBitmap("img_n.bmp");
	}
	void Render(const HDC& hdc) {
		if (fuel > 0)
		{
			m_image[0]->DrawBitmap(hdc, position.x, position.y, 0, 0);
		}
		else
			m_image[1]->DrawBitmap(hdc, position.x, position.y, 0, 0);
	}
	void Update(const _m_d_float& dt) {
		position += velocity*dt;
		if (fuel <= 0)
			return;
		_m_d_float dv = (dfuel*fuel_vel) / (mass -= dfuel * dt);

		velocity.x += dv*dt;
		fuel -= dfuel * dt;
	}

	bool CheckCollision(Collider* other, _m_d_float* overlapped) {
		return false;
	}

	~Rocket() {
		delete m_image[0];
		delete m_image[1];
	}
};

class CObjectInfoRenderer : public CObject {
private:
	CObject* m_target;
public:
	CObjectInfoRenderer(CObject* target, Vector2 pos) : m_target(target) { position = pos; }
	void Render(const HDC& hdc);
	void Update(const _m_d_float& dt) {}
	bool CheckCollision(Collider* other, _m_d_float* overlapped) { return false; }
};

class BackGroundImage : public CObject {
private:
	CBitmap* m_img;
public:
	BackGroundImage();
	~BackGroundImage();
	void Render(const HDC& hdc);
	void Update(const _m_d_float& dt);
	bool CheckCollision(Collider* other, _m_d_float* overlapped);
};
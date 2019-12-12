#include "CObject.h"

Atom::Atom(_m_d_int _radius)
{
	radius = _radius;
	colType = COLLIDER_TYPE::COLTYPE_ATOM;
}

void Atom::Render(const HDC & hdc)
{
	Ellipse(hdc, position.x - radius, position.y + radius, position.x + radius, position.y - radius);
	//Render Speed line
	MoveToEx(hdc, position.x, position.y, NULL);
	LineTo(hdc, position.x + velocity.x, position.y + velocity.y);
}

void Atom::Update(const _m_d_float & dt)
{
	position += velocity*dt;
}

bool Atom::CheckCollision(Collider * other, _m_d_float* overlapped)
{
	if (other->getColType() == COLLIDER_TYPE::COLTYPE_ATOM) {
		auto o = static_cast<Atom*>(other);
		const _m_d_float && _overlapped = (o->position - position).magnitude() - (o->radius + radius);
		*overlapped = _overlapped;
		return _overlapped <= 0;
	}
	return false;
}

CObject::CObject()
{
	mass = 1;
	charge = 0;
	position = Vector2(0, 0);
	velocity = Vector2(0, 0);
}

KineticObject::KineticObject(const Vector2 & _position, _m_d_float _mass) : Atom(4)
{
	mass = _mass;
	position = _position;
	colType = COLLIDER_TYPE::COLTYPE_IGNORE;
}

Collider::Collider(COLLIDER_TYPE type)
{
	colType = type;
}

BackGroundImage::BackGroundImage()
{
	m_img = new CBitmap("bg.bmp");
}

BackGroundImage::~BackGroundImage()
{
	delete m_img;
}

void BackGroundImage::Render(const HDC & hdc)
{
	const auto h = m_img->getInfo().bmHeight, w = m_img->getInfo().bmWidth;
	m_img->DrawBitmap(hdc, 0, 0, 0, 0);
	m_img->DrawBitmap(hdc, 0, h, 0, 0);
	m_img->DrawBitmap(hdc, w, 0, 0, 0);
	m_img->DrawBitmap(hdc, w, h, 0, 0);
}

void BackGroundImage::Update(const _m_d_float & dt)
{
}

bool BackGroundImage::CheckCollision(Collider * other, _m_d_float * overlapped)
{
	return false;
}

void CObjectInfoRenderer::Render(const HDC & hdc)
{
	char bf[64];
	sprintf_s(bf, 64, "V: %.2f M: %.2f", m_target->velocity.magnitude(), m_target->mass);
	TextOutA(hdc, position.x, position.y, bf, strlen(bf));
}

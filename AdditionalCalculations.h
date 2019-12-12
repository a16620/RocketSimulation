#pragma once
#include "CObject.h"
#include <vector>

class AdditionalCalculation
{
public:
	virtual void Perform(std::vector<CObject*>* plist, const _m_d_float& dt) = 0;
};

class GravityMovement : public AdditionalCalculation {
private:
	static inline Vector2 CalcGravity(CObject* obj1, CObject* obj2) {
		Vector2 direction = (obj2->position - obj1->position);
		const _m_d_float _distance = direction.sqrtMagnitude();
		direction = direction.unit();
		const _m_d_float force = (CONST_GRAVITY*obj1->mass*obj2->mass) / _distance;
		direction *= force;
		return direction;
	}
public:
	void Perform(std::vector<CObject*>* plist, const _m_d_float& dt) {
		for (int i = 0; i < plist->size(); ++i) {
			for (int j = 0; j < plist->size(); ++j) {
				if (i == j)
					continue;
				register Vector2 tmpVec = CalcGravity((*plist)[i], (*plist)[j]);
				tmpVec /= (*plist)[i]->mass;
				tmpVec *= dt;
				(*plist)[i]->velocity += tmpVec;
			}
		}
	}
};

class ElectroMagneticMovement : public AdditionalCalculation {
private:
	static inline Vector2 CalcForce(CObject* obj1, CObject* obj2) {
		Vector2 direction = (obj1->position - obj2->position);
		const _m_d_float _distance = direction.sqrtMagnitude();
		direction = direction.unit();
		const _m_d_float force = (CONST_ELECTROMAGNETIC*obj1->charge*obj2->charge) / _distance;
		direction *= force;
		return direction;
	}
public:
	void Perform(std::vector<CObject*>* plist, const _m_d_float& dt) {
		for (int i = 0; i < plist->size(); ++i) {
			for (int j = 0; j < plist->size(); ++j) {
				if (i == j)
					continue;
				register Vector2 tmpVec = CalcForce((*plist)[i], (*plist)[j]);
				tmpVec /= (*plist)[i]->mass;
				tmpVec *= dt;
				(*plist)[i]->velocity += tmpVec;;
			}
		}
	}
};

class GEMovement : public AdditionalCalculation {
private:
	static inline Vector2 CalcForce(CObject* obj1, CObject* obj2) {
		Vector2 direction = (obj1->position - obj2->position);
		const _m_d_float _distance = direction.sqrtMagnitude();
		direction = direction.unit();
		const _m_d_float forceG = (CONST_GRAVITY*obj1->mass*obj2->mass) / _distance;
		const _m_d_float forceE = (CONST_ELECTROMAGNETIC*obj1->charge*obj2->charge) / _distance;
		direction *= (forceE-forceG);
		return direction;
	}
public:
	void Perform(std::vector<CObject*>* plist, const _m_d_float& dt) {
		for (int i = 0; i < plist->size(); ++i) {
			for (int j = 0; j < plist->size(); ++j) {
				if (i == j)
					continue;
				register Vector2 tmpVec = CalcForce((*plist)[i], (*plist)[j]);
				tmpVec /= (*plist)[i]->mass;
				tmpVec *= dt;
				(*plist)[i]->velocity += tmpVec;
			}
		}
	}
};

class CollisionChecker {
protected:
	static inline bool IsCollied(Collider* obj1, Collider* obj2, _m_d_float* overlapped) {
		return obj1->CheckCollision(obj2, overlapped);
	}
};

class MergeCollisionMovement : public CollisionChecker, public AdditionalCalculation {
public:
	void Perform(std::vector<CObject*>* plist) {
		int length = plist->size();
		for (int i = 0; i < length; ++i) {
			for (int j = i + 1; j < length; ++j) {
				_m_d_float overlapped;
				if (IsCollied((*plist)[i], (*plist)[j], &overlapped)) {
					const _m_d_float newMass = (*plist)[j]->mass + (*plist)[i]->mass;
					Vector2 newVel = ((*plist)[j]->velocity*(*plist)[j]->mass + (*plist)[i]->velocity*(*plist)[i]->mass) / newMass;
					(*plist)[j]->mass = newMass;
					(*plist)[j]->velocity = newVel;
					TSwap((*plist)[i], (*plist)[length - 1]);
					plist->pop_back();
					length--;
				}
			}
		}
	}
};

class RepelCollisionMovement : public CollisionChecker, public AdditionalCalculation {
public:
	void Perform(std::vector<CObject*>* plist) {
		int length = plist->size();
		for (int i = 0; i < length; ++i) {
			for (int j = i + 1; j < length; ++j) {
				_m_d_float overlapped = 0;
				if (IsCollied((*plist)[i], (*plist)[j], &overlapped)) {
					CObject* const o = (*plist)[i], * const o2 = (*plist)[j];
					const Vector2 && _rp = (o->position - o2->position).unit() * overlapped * 0.5f;
					o->position -= _rp;
					o2->position += _rp;

					const Vector2 && v1 = o->velocity - (o->position-o2->position)*
						(((o->velocity - o2->velocity).ScalarProduct(o->position - o2->position)
							/(o->position - o2->position).sqrtMagnitude())*((2*o2->mass)/(o->mass+o2->mass)));
					const Vector2 && v2 = o2->velocity - (o2->position - o->position)*
						(((o2->velocity - o->velocity).ScalarProduct(o2->position - o->position)
							/ (o2->position - o->position).sqrtMagnitude())*((2 * o->mass) / (o2->mass + o->mass)));
					o->velocity = v1;
					o2->velocity = v2;
				}
			}
		}
	}
};

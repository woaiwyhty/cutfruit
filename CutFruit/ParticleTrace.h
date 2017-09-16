#pragma once
#include "stdafx.h"
#include "colorlist.h"
class ParticleTrace {
public:
	ParticleTrace() {

	}
	ParticleTrace(POINT ptPostion, POINT ptVelocity, POINT ptAcceleration, int radius, COLORREF crColor);
	~ParticleTrace();
	virtual void Draw(HDC hDC);
	virtual void Update();
public:
	COLORREF m_color;
	int m_radius, m_lifeTime, m_delay;
	BOOL m_Dying;
	POINT m_Pos, m_Velocity, m_Acceration;
};
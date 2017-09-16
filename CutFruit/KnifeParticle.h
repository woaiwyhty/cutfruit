#pragma once
#include "ParticleTrace.h"
#define PI 3.1415926
class KnifeParticle : public ParticleTrace {
public:
	KnifeParticle(POINT pos, POINT velocity, POINT acc, int radius, int longRadius, COLORREF color);
	~KnifeParticle();
	void Draw(HDC hDC);
	void Update();
protected:
	int m_nowRadius, m_longRadius;
	double theta;
};
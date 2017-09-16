#pragma once
#include "ParticleTrace.h"

class ParticleController {
public:
	ParticleController();
	~ParticleController();
	void Update();
	void Draw(HDC hDC);
	void AddParicles(int nums, POINT m_Pos, POINT m_Velocity, COLORREF m_Color);
	void ClearupParticles();
	void AddParticle(ParticleTrace *pParticle);
protected:
	vector <ParticleTrace*> m_vParicles;
};
#include "ParticleController.h"

ParticleController::ParticleController() {
	m_vParicles.reserve(300);
}

ParticleController::~ParticleController() {
	ClearupParticles();
}

void ParticleController::AddParticle(ParticleTrace *pParticle) {
	if(pParticle != NULL)
		m_vParicles.push_back(pParticle);
}

void ParticleController::AddParicles(int nums, POINT m_Pos, POINT m_Velocity, COLORREF m_Color) {
	ParticleTrace* pParticle;
	POINT pPos, pVelocity, pAcceration = {0 , 2};
	pPos = m_Pos; pVelocity = m_Velocity;
	int pRadius, rnd;

	for (int i = 0; i < nums; ++i) {
		rnd = rand() % 2;
		pVelocity.x = (rnd == 0) ? m_Velocity.x + rand() % 20 : m_Velocity.x - rand() % 20;
		rnd = rand() % 2;
		pVelocity.y = (rnd == 0) ? m_Velocity.y + rand() % 20 : m_Velocity.y - rand() % 20;

		pRadius = rand() % 20;
		if (pRadius <= 5) pRadius += 10;
		pParticle = new ParticleTrace(pPos, pVelocity, pAcceration, pRadius, m_Color);
		m_vParicles.push_back(pParticle);
	}
}

void ParticleController::ClearupParticles() {
	vector <ParticleTrace*> ::iterator it;
	for (it = m_vParicles.begin(); it != m_vParicles.end(); ++it) {
		delete (*it);
	}
	m_vParicles.clear();
}

void ParticleController::Draw(HDC hDC) {
	vector <ParticleTrace*> ::iterator it;
	for (it = m_vParicles.begin(); it != m_vParicles.end(); ++it) {
		(*it)->Draw(hDC);
	}
}

void ParticleController::Update() {
	if (m_vParicles.size() >= (m_vParicles.capacity() >> 1))
		m_vParicles.reserve(m_vParicles.capacity() * 2);

	vector <ParticleTrace*> ::iterator it;
	for (it = m_vParicles.begin(); it != m_vParicles.end(); ) {
		(*it)->Update();
		if ((*it)->m_Dying) {
			delete (*it);
			it = m_vParicles.erase(it);
		}
		else {
			++it;
		}
	}
}
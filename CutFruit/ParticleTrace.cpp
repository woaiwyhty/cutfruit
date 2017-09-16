#include "ParticleTrace.h"
ParticleTrace::ParticleTrace(POINT ptPostion, POINT ptVelocity, POINT ptAcceleration, int radius, COLORREF crColor) {
	m_Pos = ptPostion;
	m_Velocity = ptVelocity;
	m_Acceration = ptAcceleration;
	m_radius = radius;
	m_color = crColor;
	m_delay = 0;
	m_Dying = FALSE;
}

ParticleTrace::~ParticleTrace() {

}

void ParticleTrace::Update() {
	m_radius--;
	if (m_radius <= 0) {
		m_Dying = TRUE;
		return;
	}
	m_Pos.x += m_Velocity.x; m_Pos.y += m_Velocity.y;
	m_Velocity.x += m_Acceration.x; m_Velocity.y += m_Acceration.y;
}

void ParticleTrace::Draw(HDC hDC) {
	HBRUSH hBrush = CreateSolidBrush(m_color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hPen = CreatePen(PS_SOLID, 1, m_color);
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
	Ellipse(hDC, m_Pos.x, m_Pos.y, m_Pos.x + m_radius * 2, m_Pos.y + m_radius * 2);

	HBRUSH hWhiteBrush = CreateSolidBrush(WHITE);
	oldBrush = (HBRUSH)SelectObject(hDC, hWhiteBrush);
	int newRadius = m_radius / 3 + 1, newX = m_Pos.x + m_radius, newY = m_Pos.y + m_radius - newRadius - newRadius;
	Ellipse(hDC, newX, newY, newX + newRadius * 2, newY + newRadius * 2);

	DeleteObject(hBrush);
	DeleteObject(hWhiteBrush);
	DeleteObject(hPen);
	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);
}
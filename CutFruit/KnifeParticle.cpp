#include "KnifeParticle.h"

KnifeParticle::KnifeParticle(POINT pos, POINT velocity, POINT acc, int radius, int longRadius, COLORREF color):
	ParticleTrace(pos, velocity, acc, radius, color) {
	theta = (double)(rand() % 360);
	theta *= (PI / 180); //angle in rad
	m_nowRadius = 0; m_longRadius = longRadius;
}

KnifeParticle::~KnifeParticle() {

}

void KnifeParticle::Update() {
	++m_nowRadius;
	if (m_nowRadius > m_radius) {
		m_Dying = TRUE;
		return;
	}
}

void KnifeParticle::Draw(HDC hDC) {
	HBRUSH hBrush = CreateSolidBrush(m_color);
	HPEN hBluePen = CreatePen(PS_SOLID, 1, BLUE);
	HPEN oldPen = (HPEN)SelectObject(hDC, hBluePen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	double nowLongRadius = (double)m_nowRadius * (double)m_longRadius / m_radius;
	POINT pt[4];
	double cosTemp = (double)m_nowRadius * cos(theta), sinTemp = (double)m_nowRadius * sin(theta);
	pt[0].x = m_Pos.x - cosTemp; pt[0].y = m_Pos.y - sinTemp;
	pt[2].x = m_Pos.x + cosTemp; pt[2].y = m_Pos.y + sinTemp;

	cosTemp = (double)nowLongRadius * cos(theta), sinTemp = (double)nowLongRadius * sin(theta);
	pt[1].x = m_Pos.x + sinTemp; pt[1].y = m_Pos.y - cosTemp;
	pt[3].x = m_Pos.x - sinTemp; pt[3].y = m_Pos.y + cosTemp;
	HRGN hRgn = CreatePolygonRgn(pt, 4, WINDING);
	FillRgn(hDC, hRgn, hBrush);
	MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
	for (int i = 1; i < 4; ++i) {
		LineTo(hDC, pt[i].x, pt[i].y);
	}
	LineTo(hDC, pt[0].x, pt[0].y);

	DeleteObject(hBrush);
	DeleteObject(hBluePen);
	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
}
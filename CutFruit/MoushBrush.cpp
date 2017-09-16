#include "MouseBrush.h"

MouseBrush::MouseBrush(int sz /* = 5 */) {
	m_dataPoint = new QUEUE1<POINT>(sz);
	m_iTrigger = 0;
	m_isVal = TRUE;
}

MouseBrush::~MouseBrush() {
	if (m_dataPoint != NULL)
		delete m_dataPoint;
}

void MouseBrush::DrawBrush(HDC hDC) {
	if (m_dataPoint == NULL || m_dataPoint->GetSize() <= 1) return;
	POINT p1;
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
	p1 = m_dataPoint->GetAt(0);
	MoveToEx(hDC, p1.x, p1.y, NULL);
	for (int i = 1; i < m_dataPoint->GetSize(); ++i) {
		p1 = m_dataPoint->GetAt(i);
		LineTo(hDC, p1.x, p1.y);
		MoveToEx(hDC, p1.x, p1.y, NULL);
	}
	SelectObject(hDC, oldPen);
	DeleteObject(hPen);
}

void MouseBrush::Update() {
	++m_iTrigger;
	if(m_iTrigger > UPDATE_TIME) {
		Clear();
		m_iTrigger = 0;
	}
}

void MouseBrush::Clear() {
	m_dataPoint->clear();
	m_iTrigger = 0;
}

void MouseBrush::InsertPoint(int x, int y) {
	POINT pt;
	pt.x = x;
	pt.y = y;
	m_dataPoint->push_queue(pt);
	m_iTrigger = 0;
}
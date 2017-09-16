#include "Fruit.h"

Fruit::Fruit(Bitmap *pBitmap):Sprite(pBitmap), m_iTrigger(0) {
	m_ptAcceleration.x = m_ptAcceleration.y = 0;
}

Fruit::Fruit(Bitmap *pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT &rcBounds, BOUNDSACTION baBoundsAction /* = BA_STOP */) : Sprite(pBitmap, ptPosition, ptVelocity,
	iZOrder, rcBounds, baBoundsAction), m_iTrigger(0) {
	m_ptAcceleration.x = 0;
	m_ptAcceleration.y = 2;
}

Fruit::Fruit(Bitmap *pBitmap, RECT &rcBounds, BOUNDSACTION baBoundsAction /* = BA_STOP */):Sprite(pBitmap, rcBounds, baBoundsAction), m_iTrigger(0) { 
	m_ptAcceleration.x = 0;
	m_ptAcceleration.y = 2;
}

Fruit::~Fruit() {

}

SPRITEACTION Fruit::Update() {
	++m_iTrigger;
	if (m_iTrigger == 5) {
		m_iTrigger = 0;
		m_ptVelocity.y += m_ptAcceleration.y;
	}
	return Sprite::Update();
}

BOOL Fruit::MouseCollision(QUEUE1<POINT> *pMouseBrushData) {
	POINT pt1, pt2;
	if (pMouseBrushData->GetSize() < 2) return FALSE;
	pt1 = pMouseBrushData->GetAt(0);
	for (int i = 1; i < pMouseBrushData->GetSize(); ++i) {
		pt2 = pMouseBrushData->GetAt(i);

		if (pt1.x >= m_rcCollision.left && pt1.x <= m_rcCollision.right &&
			pt1.y >= m_rcCollision.top && pt1.y <= m_rcCollision.bottom)
			return TRUE;

		if (pt2.x >= m_rcCollision.left && pt2.x <= m_rcCollision.right &&
			pt2.y >= m_rcCollision.top && pt2.y <= m_rcCollision.bottom)
			return TRUE;
		int miny = min(pt1.y, pt2.y), maxy = max(pt1.y, pt2.y), tempy;
		int minx = min(pt1.x, pt2.x), maxx = max(pt1.x, pt2.x), tempx;
		// horizontal edge
		if (pt1.y != pt2.y) {
			if (pt1.x == pt2.x) {
				//perpendicular
				if (pt1.x >= m_rcCollision.left && pt1.x <= m_rcCollision.right &&
					((m_rcCollision.top >= miny && m_rcCollision.top <= maxy) ||
					(m_rcCollision.bottom >= miny && m_rcCollision.bottom <= maxy)))
					return TRUE;
			}
			else {
				// not perpendicular
				//K = (pt2.y - pt1.y) / (pt2.x - pt1.x);
				tempx = (m_rcCollision.top - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y) + pt1.x;
				if (tempx >= m_rcCollision.left && tempx <= m_rcCollision.right &&
					tempx >= minx && tempx <= maxx &&
					m_rcCollision.top >= miny && m_rcCollision.top <= maxy)
					return TRUE;

				tempx = (m_rcCollision.bottom - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y) + pt1.x;
				if (tempx >= m_rcCollision.left && tempx <= m_rcCollision.right &&
					tempx >= minx && tempx <= maxx &&
					m_rcCollision.bottom >= miny && m_rcCollision.bottom <= maxy)
					return TRUE;
			}
		}
		// vertical edge
		if (pt1.x != pt2.x) {
			if (pt1.y == pt2.y) {
				// perpendicular
				if (pt1.y >= m_rcCollision.top && pt1.y <= m_rcCollision.bottom &&
					((m_rcCollision.left >= minx && m_rcCollision.left <= maxx) ||
					(m_rcCollision.right >= minx && m_rcCollision.right <= maxx)))
					return TRUE;
			}
			else {
				// not perpendicular
				//K = (pt2.y - pt1.y) / (pt2.x - pt1.x);
				tempy = (pt2.y - pt1.y) * (m_rcCollision.left - pt1.x) / (pt2.x - pt1.x) + pt1.y;
				if (tempy >= m_rcCollision.top && tempy <= m_rcCollision.bottom &&
					tempy >= miny && tempy <= maxy &&
					m_rcCollision.left >= minx && m_rcCollision.left <= maxx)
					return TRUE;

				tempy = (pt2.y - pt1.y) * (m_rcCollision.right - pt1.x) / (pt2.x - pt1.x) + pt1.y;
				if (tempy >= m_rcCollision.top && tempy <= m_rcCollision.bottom &&
					tempy >= miny && tempy <= maxy &&
					m_rcCollision.right >= minx && m_rcCollision.right <= maxx)
					return TRUE;
			}
		}
		pt1 = pt2;
	}
	return FALSE;
}
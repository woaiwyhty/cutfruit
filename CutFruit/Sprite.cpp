#include "Sprite.h"

Sprite::Sprite(Bitmap *mBitmap) {
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = m_iZOrder = 0;
	m_pBitmap = mBitmap; m_iNumFrames = 1;
	m_bDying = FALSE; m_bOneCycle = FALSE; m_bIsClockWise = TRUE;

	SetRect(&m_rcPosition, 0, 0, mBitmap->GetWidth(), mBitmap->GetHeight());
	CalcCollisionRec();

	m_ptVelocity.x = m_ptVelocity.y = 0;
	SetRect(&m_rcBounds, 0, 0, 800, 600);

	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap *pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT &rcBounds, BOUNDSACTION baBoundsAction /* = BA_STOP */) {
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = m_iZOrder = 0;
	m_pBitmap = pBitmap; m_iNumFrames = 1;
	m_bDying = FALSE; m_bOneCycle = FALSE; m_bIsClockWise = TRUE;
	m_pBitmap = pBitmap;

	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, ptPosition.x + pBitmap->GetWidth(), ptPosition.y + pBitmap->GetHeight());
	m_ptVelocity.x = ptVelocity.x, m_ptVelocity.y = ptVelocity.y;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	CalcCollisionRec();
}

Sprite::Sprite(Bitmap *pBitmap, RECT &rcBounds, BOUNDSACTION baBoundsAction /* = BA_STOP */) {
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = m_iZOrder = 0;
	m_pBitmap = pBitmap; m_iNumFrames = 1;
	m_bDying = FALSE; m_bOneCycle = FALSE; m_bIsClockWise = TRUE;
	m_pBitmap = pBitmap;

	int x = rand() % (rcBounds.right - rcBounds.left) + rcBounds.left;
	int y = rand() % (rcBounds.bottom - rcBounds.top) + rcBounds.top;

	SetRect(&m_rcPosition, x, y, x + pBitmap->GetWidth(), y + pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	CalcCollisionRec();
}

Sprite::~Sprite() {

}

void Sprite::CalcCollisionRec() {
	int nx = (m_rcPosition.left - m_rcPosition.right) / 12;
	int ny = (m_rcPosition.top - m_rcPosition.bottom) / 12;
	CopyRect(&m_rcCollision, &m_rcPosition);
	InflateRect(&m_rcCollision, nx, ny);
}

SPRITEACTION Sprite::Update() {
	if (m_bDying) return SA_KILL;
	
	UpdateFrame();

	POINT ptNewpos;
	ptNewpos.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewpos.y = m_rcPosition.top + m_ptVelocity.y;

	POINT ptSprite;
	ptSprite.x = m_rcPosition.right - m_rcPosition.left;
	ptSprite.y = m_rcCollision.bottom - m_rcPosition.top;

	POINT ptBounds;
	ptBounds.x = m_rcBounds.right - m_rcBounds.left;
	ptBounds.y = m_rcBounds.bottom - m_rcBounds.top;

	if (m_baBoundsAction == BA_DIE) {
		if (m_rcPosition.left + ptSprite.x <= m_rcBounds.left || m_rcPosition.right - ptSprite.x >= m_rcCollision.right
			|| m_rcPosition.top + ptSprite.y <= m_rcBounds.top || m_rcPosition.bottom - ptSprite.y >= m_rcBounds.bottom)
			return SA_KILL;
	}
	SetPosition(ptNewpos);
	CalcCollisionRec();
	return SA_NONE;
}

Sprite* Sprite::AddSprite() {
	return NULL;
}

void Sprite::UpdateFrame() {
	if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger) <= 0) {
		m_iFrameTrigger = m_iFrameDelay;
		if (m_bIsClockWise) {
			if (++m_iCurFrame >= m_iNumFrames) {
				if (m_bOneCycle) m_bDying = TRUE;
				else m_iCurFrame = 0;
			}
		}
		else {
			if (--m_iCurFrame <= -1) {
				if (m_bOneCycle) m_bDying = TRUE;
				else m_iCurFrame = m_iNumFrames - 1;
			}
		}
	}
}

void Sprite::Draw(HDC hDC) {
	if (m_pBitmap != NULL && !m_bHidden) {
		if (m_iNumFrames == 1)
			m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
		else
			m_pBitmap->DrawPart(hDC, m_rcPosition.left, m_rcPosition.top, 0, m_iCurFrame * GetHeight(), GetWidth(), GetHeight(), TRUE);
	}
}

BOOL Sprite::MouseCollision(QUEUE1<POINT> *pMouseBrushData) {
	return FALSE;
}

void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle)
{
	// Set the number of frames and the one-cycle setting
	m_bOneCycle = bOneCycle;
	m_iNumFrames = iNumFrames;
	RECT rect = GetPosition();
	rect.bottom = rect.top + ((rect.bottom - rect.top) / iNumFrames);
	SetPosition(rect);
}

void Sprite::SetPosition(const POINT &ptPosition)
{
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
		ptPosition.y - m_rcPosition.top);
	CalcCollisionRec();
}

void Sprite::SetPosition(int x, int y) {
	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
	CalcCollisionRec();
}

void Sprite::OffsetPosition(int x, int y) {
	m_rcPosition.left += x, m_rcPosition.right += x;
	m_rcPosition.top += y, m_rcPosition.bottom += y;
}

void Sprite::SetVelocity(int x, int y) {
	m_ptVelocity.x = x, m_ptVelocity.y = y;
}

BOOL Sprite::IsPointInside(const POINT &pt) {
	if (pt.x >= m_rcPosition.left && pt.x <= m_rcPosition.right
		&& pt.y >= m_rcPosition.top && pt.y <= m_rcPosition.bottom)
		return TRUE;
	return FALSE;
}

BOOL Sprite::IsPointInside(int x, int y) {
	if (x >= m_rcPosition.left && x <= m_rcPosition.right
		&& y >= m_rcPosition.top && y <= m_rcPosition.bottom)
		return TRUE;
	return FALSE;
}

BOOL Sprite::IsTouchBoundX() {
	if (m_rcPosition.left <= m_rcBounds.left || m_rcPosition.right >= m_rcBounds.right)
		return TRUE;
	return FALSE;
}

BOOL Sprite::IsTouchBoundY() {
	if (m_rcPosition.top <= m_rcBounds.top || m_rcPosition.bottom >= m_rcBounds.bottom)
		return TRUE;
	return FALSE;
}

Bitmap* Sprite::GetBitmap() {
	return m_pBitmap;
}

void Sprite::SetFrameDelay(int iFrameDelay) {
	m_iFrameDelay = iFrameDelay;
}

BOOL Sprite::TestCollision(Sprite *pTestSprite) {
	RECT rc = pTestSprite->m_rcCollision;
	if (rc.left <= m_rcCollision.right && rc.right >= m_rcCollision.left && rc.top <= m_rcCollision.bottom && rc.bottom >= m_rcCollision.top)
		return TRUE;
	return FALSE;
}


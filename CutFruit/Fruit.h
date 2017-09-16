#pragma once
#include "Sprite.h"
class Fruit : public Sprite {
public:
	Fruit(Bitmap *pBitmap);
	Fruit(Bitmap *pBitmap, RECT &rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	Fruit(Bitmap *pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT &rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	~Fruit();

	BOOL MouseCollision(QUEUE1<POINT> *pMouseBrushData);

	SPRITEACTION Update();

protected:
	POINT m_ptAcceleration;	
	int m_iTrigger;			
};
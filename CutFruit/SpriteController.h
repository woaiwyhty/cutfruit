#pragma once
#include "Sprite.h"
#include "MouseBrush.h"

class SpriteController {
public:
	SpriteController(MouseBrush *pMouseBrush);
	~SpriteController(void);

protected:
	BOOL CheckSpriteCollision(Sprite * pTestSprite);

public:
	void AddSprite(Sprite *pSprite);	// Add a sprite
	void DrawSprites(HDC hDC);			// draw all the sprites
	virtual void UpdateSprites();		// update all the sprites
	void ClearupSprite();				// clear the vector
	Sprite * IsPointInSprite(int x, int y);	// get the sprite which the point is in
	Sprite * IsPointInSprite(const POINT &point);

protected:
	// 子画面管理器
	vector<Sprite *> m_vSprites;
	// 这个变量是由外部设置的，生命周期跟这个类无关
	MouseBrush *m_pMouseBrush;
};
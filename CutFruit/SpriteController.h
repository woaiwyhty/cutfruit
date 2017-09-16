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
	// �ӻ��������
	vector<Sprite *> m_vSprites;
	// ������������ⲿ���õģ��������ڸ�������޹�
	MouseBrush *m_pMouseBrush;
};
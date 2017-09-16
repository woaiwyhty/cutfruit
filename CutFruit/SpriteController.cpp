#include "SpriteController.h"

SpriteController::SpriteController(MouseBrush *pMouseBrush) {
	m_pMouseBrush = pMouseBrush;
	m_vSprites.reserve(100);
}

SpriteController::~SpriteController() {
	ClearupSprite();
}

void SpriteController::AddSprite(Sprite *pSprite) {
	if(pSprite != NULL)
		m_vSprites.push_back(pSprite);
}

void SpriteController::DrawSprites(HDC hDC) {
	vector <Sprite*> ::iterator it;
	for (it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
		(*it)->Draw(hDC);
	}
}

void SpriteController::UpdateSprites() {

}

void SpriteController::ClearupSprite() {
	vector <Sprite*> ::iterator it;
	for (it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
		delete (*it);
	}
	m_vSprites.clear();
}

Sprite* SpriteController::IsPointInSprite(int x, int y) {
	vector <Sprite*> ::iterator it;
	for (it = m_vSprites.begin(); it != m_vSprites.end(); ++it) {
		RECT rc = (*it)->GetPosition();
		if (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom)
			return (*it);
	}
	return NULL;
}
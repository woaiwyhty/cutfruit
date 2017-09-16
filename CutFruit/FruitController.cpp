#include "FruitController.h"
extern void MouseCollisionAction(Sprite *pSprite);
FruitController::FruitController(MouseBrush *pMouseBrush):SpriteController(pMouseBrush) {

}

FruitController::~FruitController() {
	ClearupSprite();
}

void FruitController::UpdateSprites() {
	if (m_vSprites.size() >= m_vSprites.capacity() / 2)
		m_vSprites.reserve(m_vSprites.capacity() * 2);

	vector <Sprite*> ::iterator it;
	RECT rc; SPRITEACTION mAction;
	QUEUE1<POINT> *pMouseBrushData = m_pMouseBrush->GetData();
	for (it = m_vSprites.begin(); it != m_vSprites.end(); ) {
		rc = (*it)->GetPosition();
		mAction = (*it)->Update();

		if ((*it)->MouseCollision(pMouseBrushData) && m_pMouseBrush->GetMouseValid() == TRUE) {
			mAction = SA_KILL;
			MouseCollisionAction((*it));
		}

		if (mAction == SA_KILL) {
			delete (*it);
			it = m_vSprites.erase(it);
		}
		else ++it;
	}
}
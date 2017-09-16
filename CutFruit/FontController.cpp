#include "FontController.h"

FontController::FontController() {
	m_vFont.reserve(100);
}

FontController::~FontController() {
	ClearupFonts();
}

void FontController::AddContent(GameFont* pFont) {
	if (pFont != NULL)
		m_vFont.push_back(pFont);
}

void FontController::ClearupFonts() {
	vector <GameFont*> ::iterator it;
	for (it = m_vFont.begin(); it != m_vFont.end(); ++it)
		delete (*it);
	m_vFont.clear();
}

void FontController::Draw(HDC hDC) {
	vector <GameFont*> ::iterator it;
	for (it = m_vFont.begin(); it != m_vFont.end(); ++it) {
		(*it)->Draw(hDC);
	}
}

void FontController::Update() {
	if (m_vFont.size() > m_vFont.capacity() / 2)
		m_vFont.reserve(m_vFont.capacity() * 2);

	vector <GameFont*> ::iterator it;
	for (it = m_vFont.begin(); it != m_vFont.end();  ) {
		(*it)->Update();

		if ((*it)->isDying()) {
			delete (*it);
			it = m_vFont.erase(it);
		}
		else ++it;
	}
}
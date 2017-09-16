#pragma once
#include "Gamefont.h"

class FontController {
public:
	FontController();
	~FontController();
	void AddContent(GameFont* pFont);
	void Update();
	void Draw(HDC hDC);
	void ClearupFonts();
protected:
	vector <GameFont*> m_vFont;
};
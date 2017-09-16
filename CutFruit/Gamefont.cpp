#include "Gamefont.h"

GameFont::GameFont(const RECT mPostion, POINT velocity, COLORREF mBackColor, COLORREF mFrontColor, int mLifeCycle) {
	m_posrect = mPostion;
	m_backcolor = mBackColor;
	m_frontcolor = mFrontColor;
	m_lifeCycle = mLifeCycle;
	m_velocity = velocity;
	szContent = new TCHAR[64];
	m_font = CreateFont(50, 0, 1, 1, 1200, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Î¢ÈíÑÅºÚ");
}
GameFont::~GameFont() {
	if (szContent != NULL) delete szContent;
}

void GameFont::Draw(HDC hDC) {
	HFONT oldFont = (HFONT)SelectObject(hDC, m_font);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, m_backcolor);
	DrawText(hDC, szContent, -1, &m_posrect, DT_SINGLELINE | DT_CENTER);

	//shadow
	m_posrect.left += 2; m_posrect.right += 2;
	SetTextColor(hDC, m_frontcolor);
	DrawText(hDC, szContent, -1, &m_posrect, DT_SINGLELINE | DT_CENTER);

	m_posrect.left -= 2; m_posrect.right -= 2;
	SelectObject(hDC, oldFont);
}

void GameFont::Update() {
	m_lifeCycle--;
	m_posrect.left += m_velocity.x; m_posrect.right += m_velocity.x;
	m_posrect.top += m_velocity.y; m_posrect.bottom += m_velocity.y;
}

void GameFont::SetContent(TCHAR *pContent) {
	if (szContent != NULL)
		delete szContent;
	szContent = pContent;
}

void GameFont::SetVelocity(POINT v) {
	m_velocity = v;
}
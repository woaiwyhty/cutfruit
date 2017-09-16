#pragma once
#include "stdafx.h"
/*
Show score and time during the game.
*/
class GameFont {
public:
	GameFont(const RECT mPostion, POINT velocity, COLORREF mBackColor, COLORREF mFrontColor, int mLifeCycle);
	~GameFont();

	void SetFrontColor(COLORREF clr) { m_frontcolor = clr; }
	void SetBackColor(COLORREF clr) { m_backcolor = clr; }
	COLORREF GetBackColor() { return m_backcolor; }
	COLORREF GetFrontColor() { return m_frontcolor; }
	void SetPostion(RECT  rct) { m_posrect = rct; }
	RECT GetPostion() { return m_posrect; }
	void SetContent(TCHAR *pContent);
	TCHAR* GetContent() { return szContent; }
	void SetLifeCyle(int x) { m_lifeCycle = x; }
	virtual void SetVelocity(POINT v);
	virtual void Draw(HDC hDC);
	virtual void Update();
	BOOL isDying() { return m_lifeCycle == 0; }
protected:
	RECT m_posrect;
	COLORREF  m_frontcolor, m_backcolor;
	TCHAR *szContent;
	int m_lifeCycle;
	HFONT m_font; POINT m_velocity;
};
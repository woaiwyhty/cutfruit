#pragma once
#include "stdafx.h"
#pragma comment(lib, "Msimg32.lib")
class Bitmap {
protected:
	int m_width, m_height;
	HBITMAP m_bitmap;
	void Free();
public:
	Bitmap();
	Bitmap(HDC hDC, LPTSTR szFileName);
	Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInst);
	Bitmap(HDC hDC, int mWidth, int mHeight, COLORREF crColor = RGB(0, 0, 0));
	virtual ~Bitmap();

	BOOL Create(HDC hDC, LPTSTR szFileName);
	BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInst);
	BOOL Create(HDC hDC, int mWidth, int mHeight, COLORREF crColor);

	virtual void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE, COLORREF crColor = RGB(255, 0, 255));
	void DrawPart(HDC hDC, int x, int y, int cx, int cy, 
		int mWidth, int mHeight, BOOL bTrans = FALSE, COLORREF crColor = RGB(255, 0, 255));

	int GetWidth() { return m_width; }
	int GetHeight() {return m_height; }
	const HBITMAP GetHBitmap() { return m_bitmap; }
};
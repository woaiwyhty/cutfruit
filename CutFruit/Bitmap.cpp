#include "Bitmap.h"

Bitmap::Bitmap() : m_bitmap(NULL), m_height(0), m_width(0){

}

Bitmap::Bitmap(HDC hDC, LPTSTR szFileName) : m_bitmap(NULL), m_height(0), m_width(0) {
	Create(hDC, szFileName);
}

Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInst) : m_bitmap(NULL), m_height(0), m_width(0) {
	Create(hDC, uiResID, hInst);
}

Bitmap::Bitmap(HDC hDC, int mWidth, int mHeight, COLORREF crColor /* = RGB(0, 0, 0) */) : m_bitmap(NULL), m_height(0), m_width(0) {
	Create(hDC, mWidth, mHeight, crColor);
}

BOOL Bitmap::Create(HDC hDC, LPTSTR szFileName) {
	Free();

	HANDLE mFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (mFile == INVALID_HANDLE_VALUE) return FALSE;

	BITMAPFILEHEADER m_Header;
	DWORD dwRead = 0;
	BOOL bRet = ReadFile(mFile, &m_Header, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	if (!bRet) goto label1;
	if (dwRead != sizeof(BITMAPFILEHEADER) || m_Header.bfType != 0x4D42) goto label1;

	BITMAPINFO *m_bitInfo = new BITMAPINFO;
	if (m_bitInfo != NULL) {
		bRet = ReadFile(mFile, m_bitInfo, sizeof(BITMAPINFO), &dwRead, NULL);
		if (!bRet || dwRead != sizeof(BITMAPINFO)) goto label1;

		m_width = (int)m_bitInfo->bmiHeader.biWidth;
		m_height = (int)m_bitInfo->bmiHeader.biHeight;

		PBYTE mByte;
		m_bitmap = CreateDIBSection(hDC, m_bitInfo, DIB_RGB_COLORS, (PVOID*)&mByte, NULL, 0);
		if (m_bitmap != NULL && mByte != NULL) {
			SetFilePointer(mFile, m_Header.bfOffBits, NULL, FILE_BEGIN);
			bRet = ReadFile(mFile, mByte, m_bitInfo->bmiHeader.biSizeImage, &dwRead, NULL);
			if (bRet) return TRUE;
			goto label1;
		}
		else goto label1;
	}
	else goto label1;

label1:
	Free();
	CloseHandle(mFile);
	return FALSE;
}

BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInst) {
	Free();

	HRSRC src = FindResource(NULL, MAKEINTRESOURCE(uiResID), RT_BITMAP);
	if (src == NULL) return FALSE;

	HGLOBAL hMemBitmap = LoadResource(NULL, src);
	if (hMemBitmap == NULL) return FALSE;

	PBYTE hFirstAddr = (PBYTE)LockResource(hMemBitmap);
	if (hFirstAddr == NULL) {
		FreeResource(hMemBitmap);
		return FALSE;
	}

	BITMAPINFO *m_BitmapInfo = (BITMAPINFO*)hFirstAddr;
	m_width = (int)m_BitmapInfo->bmiHeader.biWidth;
	m_height = (int)m_BitmapInfo->bmiHeader.biHeight;
	PBYTE  mBuffer;
	m_bitmap = CreateDIBSection(hDC, m_BitmapInfo, DIB_RGB_COLORS, (PVOID*)&mBuffer, NULL, 0);
	if (m_bitmap != NULL && mBuffer != NULL) {
		PBYTE mTempImg = hFirstAddr + m_BitmapInfo->bmiHeader.biSize + m_BitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
		CopyMemory(mBuffer, mTempImg, m_BitmapInfo->bmiHeader.biSizeImage);

		UnlockResource(hMemBitmap);
		FreeResource(hMemBitmap);
		return TRUE;
	}
	UnlockResource(hMemBitmap);
	FreeResource(hMemBitmap);
	Free();
	return FALSE;
}

BOOL Bitmap::Create(HDC hDC, int mWidth, int mHeight, COLORREF crColor) {
	Free();
	HDC hMemDC = CreateCompatibleDC(hDC);
	if (hMemDC == NULL) return FALSE;

	m_bitmap = CreateCompatibleBitmap(hDC, mWidth, mHeight);
	if (m_bitmap == NULL) {
		DeleteDC(hMemDC);
		return FALSE;
	}
	m_width = mWidth, m_height = mHeight;
	
	HBRUSH hBrush = CreateSolidBrush(crColor);

	HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC, m_bitmap);
	RECT mRect = { 0 }; mRect.bottom = mHeight, mRect.right = mWidth;
	FillRect(hMemDC, &mRect, hBrush);

	SelectObject(hMemDC, oldBitmap);
	DeleteDC(hMemDC);
	DeleteObject(hBrush);

	return TRUE;
}

void Bitmap::Draw(HDC hDC, int x, int y, BOOL bTrans /* = FALSE */, COLORREF crColor /* = RGB(255, 0, 255) */) {
	DrawPart(hDC, x, y, 0, 0, m_width, m_height, bTrans, crColor);
}

void Bitmap::DrawPart(HDC hDC, int x, int y, int cx, int cy, int mWidth, int mHeight, BOOL bTrans, COLORREF crColor) {
	if (m_bitmap != NULL) {
		HDC hMemDC = CreateCompatibleDC(hDC);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC, m_bitmap);

		if (bTrans)
			TransparentBlt(hDC, x, y, mWidth, mHeight, hMemDC, cx, cy, mWidth, mHeight, crColor);
		else
			BitBlt(hDC, x, y, mWidth, mHeight, hMemDC, cx, cy, SRCCOPY);

		SelectObject(hMemDC, oldBitmap);
		DeleteDC(hMemDC);
	}
}


void Bitmap::Free() {
	if (m_bitmap != NULL) {
		DeleteObject(m_bitmap);
		m_bitmap = NULL;
	}
}
Bitmap::~Bitmap() {
	Free();
}
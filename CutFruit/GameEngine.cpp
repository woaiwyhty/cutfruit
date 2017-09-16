
#include "GameEngine.h"

GameEngine::GameEngine(LPTSTR szWndTitle, int cx, int cy, int iFrameRate)
{
	m_hWnd = NULL;
	m_cxGame = cx;
	m_cyGame = cy;
	m_iFrameDelay = 1000 / iFrameRate;
	m_bPause = FALSE;
	m_hBitmap = NULL;
	m_hdcMem = NULL;
	m_szWndTitle = szWndTitle;

	m_pMouseBrush = new MouseBrush(10);
	m_pSpriteController = new FruitController(m_pMouseBrush);
	m_pParticleController = new ParticleController;
	m_pFontController = new FontController;
	m_pKnifeController = new ParticleController;
}
GameEngine::~GameEngine(void)
{
	if (m_hBitmap)
	{
		DeleteObject(m_hBitmap);
	}
	if (m_hdcMem)
	{
		DeleteObject(m_hdcMem);
	}

	if (m_pSpriteController != NULL)
		delete m_pSpriteController;
	if (m_pMouseBrush != NULL)
		delete m_pMouseBrush;
	if (m_pParticleController != NULL)
		delete m_pParticleController;
	if (m_pFontController != NULL)
		delete m_pFontController;
	if (m_pKnifeController != NULL)
		delete m_pKnifeController;
}
BOOL GameEngine::CreateWnd(HINSTANCE hInst)
{
	//创建窗口类
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = m_hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_szWndTitle;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//注册窗口类
	if (!RegisterClass(&wc))
	{
		return FALSE;
	}

	//根据游戏画面计算窗口大小和位置
	int cxWnd = m_cxGame + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	int cyWnd = m_cyGame + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	if (wc.lpszMenuName != NULL)
	{
		cyWnd += GetSystemMetrics(SM_CYMENU);
	}
	int x = (GetSystemMetrics(SM_CXSCREEN) - cxWnd) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - cyWnd) / 2;
	//创建窗口
	m_hWnd = CreateWindow(m_szWndTitle, m_szWndTitle, WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, x, y, cxWnd, cyWnd, NULL, NULL, m_hInst, NULL);
	if (!m_hWnd)
	{
		int ret = GetLastError();
		return FALSE;
	}
	return TRUE;

}

LRESULT GameEngine::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_CREATE:
		GameStart(hWnd);
		return 0;
	case WM_SETFOCUS:
		m_bPause = FALSE;
		return 0;
	case WM_KILLFOCUS:
		m_bPause = FALSE;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//BitBlt(hdc, 0, 0, m_cxGame, m_cyGame, m_hdcMem, 0, 0, SRCCOPY);
		//GamePaint();
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		OnMouseLButtonDown(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONUP:
		OnMouseLButtonUp(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_RBUTTONDOWN:
		OnMouseRButtonDown(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_RBUTTONUP:
		OnMouseRButtonUp(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_DESTROY:
		GameEnd();
		PostQuitMessage(0);
		exit(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int GameEngine::Run(void) {
	MSG msg;
	int mDelay = 0;
	while (TRUE) {
		if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (!m_bPause) {
				//正在运行
				int mTick = GetTickCount();
				if (mTick >= mDelay) {
					mDelay = mTick + GetFramDelay();
					GameCycle(m_hWnd);
				}
			}
		}
	}
	return 0;
}

BOOL GameEngine::CreateBackBuffer(void) {

	HDC m_hdc = GetDC(m_hWnd);

	m_hdcMem = CreateCompatibleDC(m_hdc);
	if (m_hdcMem == NULL) return FALSE;

	m_hBitmap = CreateCompatibleBitmap(m_hdc, m_cxGame, m_cyGame);
	if (m_hBitmap == NULL) return FALSE;

	SelectObject(m_hdcMem, m_hBitmap);
	ReleaseDC(m_hWnd, m_hdc);
	return TRUE;
}

BOOL GameEngine::Init(HINSTANCE hInst) {
	if (!CreateWnd(hInst)) return FALSE;

	if (!CreateBackBuffer()) return FALSE;

	if (!SetGameParams()) return FALSE;

	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
	m_hInst = hInst;
	return TRUE;
}

void GameEngine::SetFrameRate(int iFrameRate) {
	m_iFrameDelay = 1000 / iFrameRate;
}

BOOL GameEngine::SetGameParams() {
	
	return TRUE;
}

void GameEngine::PaintToMemDC() {

}

void GameEngine::ChangeGameParams() {
	return;
}

void GameEngine::OnkeyPress(void) {

}//处理按键
void GameEngine::OnMouseLButtonDown(int x, int y) {
	m_pMouseBrush->InsertPoint(x, y);
}//处理鼠标左键按下
void GameEngine::OnMouseLButtonUp(int x, int y) {
	m_pMouseBrush->Clear();
}//处理鼠标左键释放
void GameEngine::OnMouseRButtonDown(int x, int y) {

}//处理鼠标右键按下
void GameEngine::OnMouseRButtonUp(int x, int y) {

}//处理鼠标右键释放
void GameEngine::OnMouseMove(int x, int y) {
	if (GetAsyncKeyState(VK_LBUTTON))
		m_pMouseBrush->InsertPoint(x, y);
}//处理鼠标移动


void GameEngine::AddSprite(Sprite *pSprite) {
	m_pSpriteController->AddSprite(pSprite);
}

void GameEngine::DrawSprites(HDC hDC) {
	m_pSpriteController->DrawSprites(hDC);
	m_pMouseBrush->DrawBrush(hDC);
	m_pParticleController->Draw(hDC);
	m_pFontController->Draw(hDC);
	m_pKnifeController->Draw(hDC);
}

void GameEngine::UpdateSprites() {
	m_pSpriteController->UpdateSprites();
	m_pMouseBrush->Update();
	m_pParticleController->Update();
	m_pFontController->Update();
	m_pKnifeController->Update();
}

void GameEngine::ClearupSprite() {
	m_pSpriteController->ClearupSprite();
	m_pParticleController->ClearupParticles();
	m_pFontController->ClearupFonts();
	m_pKnifeController->ClearupParticles();
}


GameEngine *pEngine = new GameEngine(TEXT("MyCutFruitGame"), 800, 600, 36);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return pEngine->HandleEvent(hWnd, msg, wParam, lParam);;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	if (pEngine == NULL) return 0;

	if (!pEngine->Init(hInstance)) {
		MessageBox(NULL, TEXT("游戏引擎初始化失败！"), TEXT("失败"), MB_ICONERROR);
		delete pEngine;
		return 0;
	}


	pEngine->Run();

	return 0;
}
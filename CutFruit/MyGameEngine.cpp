#include "MyGameEngine.h"
Bitmap *m_bitmapBackground;
Bitmap *m_bitmapStartGameBack;
Bitmap *m_bitmapGaming;
Bitmap *m_bitmapNow;

Bitmap *g_pStrawbarryBitmap;		// 草莓位图
Bitmap *g_pAppleBitmap;				// 苹果位图
Bitmap *g_pWaterMelonBitmap;		// 西瓜位图
Bitmap *g_pBroccoliBitmap;			// 西兰花位图
Bitmap *g_pPeaBitmap;				// 豌豆位图
Bitmap *g_pOnionBitmap;				// 洋葱位图
Bitmap *g_pPimentoBitmap;			// 甜椒位图
Bitmap *g_pEggplantBitmap;			// 茄子位图
Bitmap *g_pPawpawBitmap;			// 木瓜位图
Bitmap *g_pNewGameBitmap;			// 文字new game环绕文字
Bitmap *g_pBomBitmap;				// 炸弹位图
HBITMAP g_hOffscreenBitmap;
Sprite *g_pWaterMelonSprite, *g_pBomSprite;

HDC g_hMemDC;
HFONT m_hiScoreFont;

GameFont *m_score, *m_time, *m_readytime;
TCHAR *szScore = new TCHAR[32];

GameLogicController *m_pGameLogicController;

int g_iHiScores[10], stTime, nowScore, fruitDelay, gameDelay;//Score Rank

BOOL isGameOver;
void GameStart(HWND m_hWnd) {
	HDC hDC = GetDC(m_hWnd);
	g_hMemDC = CreateCompatibleDC(hDC);
	g_hOffscreenBitmap = CreateCompatibleBitmap(hDC, 800, 600);
	SelectObject(g_hMemDC, g_hOffscreenBitmap);

	m_bitmapBackground = new Bitmap(hDC, TEXT("res/BACKGROUND_PLAYING1.bmp"));
	m_bitmapStartGameBack = new Bitmap(hDC, TEXT("res/BACKGROUND_MAIN.bmp"));
	m_bitmapGaming = new Bitmap(hDC, TEXT("res/BACKGROUND_PLAYING.bmp"));
	m_bitmapNow = m_bitmapBackground;

	g_pStrawbarryBitmap = new Bitmap(hDC, L"res/STRAWBARRY.bmp");
	g_pAppleBitmap = new Bitmap(hDC, L"res/APPLE.bmp");
	g_pWaterMelonBitmap = new Bitmap(hDC, L"res/WATERMELON.bmp");

	g_pBroccoliBitmap = new Bitmap(hDC, L"res/BROCCOLI.bmp");
	g_pPeaBitmap = new Bitmap(hDC, L"res/PEA.bmp");
	g_pOnionBitmap = new Bitmap(hDC, L"res/ONION.bmp");
	g_pPimentoBitmap = new Bitmap(hDC, L"res/PIMENTO.bmp");
	g_pEggplantBitmap = new Bitmap(hDC, L"res/EGGPLANT.bmp");
	g_pNewGameBitmap = new Bitmap(hDC, L"res/MAIN_NEWGAME.bmp");
	g_pBomBitmap = new Bitmap(hDC, L"res/BOM.bmp");
	g_pPawpawBitmap = new Bitmap(hDC, L"res/PAWPAW.bmp");

	RECT rcPos = { 50 , 20 , 300 , 70 }; POINT velocity = { 0 , 0 };
	m_score = new GameFont(rcPos, velocity, RGB(255,0,0), RGB(255,255,0), -1);
	rcPos.left = 450, rcPos.right = 750;
	m_time = new GameFont(rcPos, velocity, RGB(255, 0, 0), RGB(255, 255, 0), -1);
	rcPos.left = 300, rcPos.top = 300, rcPos.bottom = 500, rcPos.right = 500;
	m_readytime = new GameFont(rcPos, velocity, RGB(255, 0, 0), RGB(255, 255, 0), -1);

	m_pGameLogicController = GameLogicController::GetInstance();

	m_hiScoreFont = CreateFont(20, 0, 1, 1, 1200, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("微软雅黑"));
	ReadHiScores();
	MainGame();
}

void GamePaint(HDC hDC) {
	m_bitmapNow->Draw(hDC, 0, 0, 0);
	//g_pWaterMelonSprite->Draw(hDC);
	pEngine->DrawSprites(hDC);

	if (m_pGameLogicController->GetCurrentState() == GS_GAMING) {
		int timeInterval = GetTickCount() - stTime;
		wsprintf(m_score->GetContent(), TEXT("SCORE:%d"), nowScore);
		m_score->Draw(hDC);
		if (timeInterval > 4000) {
			wsprintf(m_time->GetContent(), TEXT("TIME:%d"), (timeInterval - 4000) / 1000);
			m_time->Draw(hDC);
		}
		else {
			wsprintf(m_time->GetContent(), TEXT("TIME:0"));
			m_time->Draw(hDC);
			if (timeInterval > 3000)
				wsprintf(m_readytime->GetContent(), TEXT("Start!"));
			else
				wsprintf(m_readytime->GetContent(), TEXT("%d!"), (3000 - timeInterval + 1000) / 1000);
			m_readytime->Draw(hDC);
		}
	}
	else {
		//Draw High scores
		TCHAR szContent[100];
		RECT rc = { 600, 100, 750, 200 };
		SelectObject(hDC, m_hiScoreFont);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(30, 100, 50));
		wsprintf(szContent, TEXT("当前高分排行榜"));
		DrawText(hDC, szContent, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		rc.top += 20; rc.bottom += 20;
		for (int i = 0; i < 10; ++i) {
			wsprintf(szContent, TEXT("%d"), g_iHiScores[i]);
			DrawText(hDC, szContent, -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			rc.top += 20; rc.bottom += 20;
		}
	}
}

void GameCycle(HWND m_hWnd) {
	HDC hDC = GetDC(m_hWnd);
	GamePaint(g_hMemDC);

	BitBlt(hDC, 0, 0, 800, 600, g_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hDC);
	++gameDelay;
	if (gameDelay > 30) {
		pEngine->GetMouseBrush()->SetMouseValid(TRUE);
	}

	if (m_pGameLogicController->GetCurrentState() == GS_GAMING && GetTickCount() - stTime > 4000) {
		++fruitDelay;

		if (fruitDelay > 20) {
			fruitDelay = 0;
			int nums = rand() % 2, x;

			Sprite *m_Sprite;
			POINT mPos, mVelocity;
			RECT rcBounds = { 0, 0, 800, 600 };


			x = rand() % 5 + 15;
			mVelocity.x = (rand() % 2 == 0 ? rand() % 4 : -rand() % 4), mVelocity.y = -x;

			for (int i = 0; i < nums; ++i) {
				x = rand() % 650;
				if (x <= 150) x += 500;
				mPos.x = x, mPos.y = 600;
				m_Sprite = new Fruit(GetRandomFruit(), mPos, mVelocity, 0, rcBounds, BA_DIE);
				m_Sprite->SetNumFrames(15);
				m_Sprite->SetFrameDelay(4);
				m_Sprite->SetClockDirection(rand() % 2);
				pEngine->AddSprite(m_Sprite);
			}
		}

		if (isGameOver == TRUE) {
			m_pGameLogicController->popState();
			pEngine->ClearupSprite();
			STARTGAME(GS_GAMING);
			isGameOver = FALSE;
		}
	}
	pEngine->UpdateSprites();
}

BOOL WriteHiScores()
{
	HANDLE hFile = CreateFile(L"HiScores.dat", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	TCHAR cData[21];
	DWORD dwBytesWritten;

	for (int i = 0; i < 10; i++)
	{
		wsprintf(cData, L"%d", g_iHiScores[i]);
		if (!WriteFile(hFile, &cData, 20, &dwBytesWritten, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
	return CloseHandle(hFile);
}

BOOL ReadHiScores()
{
	HANDLE hFile = CreateFile(L"HiScores.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		for (int i = 0; i < 10; i++)
			g_iHiScores[i] = 0;
		return FALSE;
	}

	TCHAR cData[21];
	DWORD dwBytesRead;
	for (int i = 0; i < 10; i++)
	{
		if (!ReadFile(hFile, &cData, 20, &dwBytesRead, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		g_iHiScores[i] = _ttoi(cData);
	}
	return CloseHandle(hFile);
}

void MouseCollisionAction(Sprite *pSprite) {
	int ParticlesRange = 100, numParticles, addScore = 0;
	Bitmap *pBitmap = pSprite->GetBitmap();
	RECT rc; POINT velocity = { 1 , 1 };
	COLORREF color = RED;
	if (m_pGameLogicController->GetCurrentState() == GS_GAMING) {
		if (pBitmap == g_pStrawbarryBitmap) addScore = 1, color = RED;
		else if (pBitmap == g_pAppleBitmap) addScore = 2, color = APPLEJUICE;
		else if (pBitmap == g_pBomBitmap) {
			//game over;
			//pEngine->ClearupSprite();
			//STARTGAME(GS_GAMING);
			isGameOver = TRUE;
			return;
		}
		else if (pBitmap == g_pBroccoliBitmap) addScore = 3, color = GRASSGREEN;
		else if (pBitmap == g_pEggplantBitmap) addScore = 4, color = EGGPLANTPURPLE;
		else if (pBitmap == g_pOnionBitmap) addScore = 2, color = ONIONPURPLE;
		else if (pBitmap == g_pPeaBitmap) addScore = 3, color = GRASSGREEN;
		else if (pBitmap == g_pPimentoBitmap) addScore = 4, color = SUNFLOWER;
		else if (pBitmap == g_pWaterMelonBitmap) addScore = 2, color = RED;

		nowScore += addScore;
		rc = pSprite->GetPosition(); 
		rc.bottom = rc.top + 50; rc.right = rc.left + 100;
		GameFont *m_tempFont = new GameFont(rc, velocity, RGB(255, 255, 0), color, 30);
		wsprintf(m_tempFont->GetContent(), TEXT("+%d"), addScore);
		pEngine->GetFontController()->AddContent(m_tempFont);
	}
	else if (m_pGameLogicController->GetCurrentState() == GS_MAIN) {
		if (pBitmap == g_pNewGameBitmap) {
			m_pGameLogicController->pushState(GS_STARTGAME);
			color = RED;
			STARTGAME(GS_MAIN);
		}
		else if (pBitmap == g_pBomBitmap) {
			::PostQuitMessage(0);
		}
	}
	else if (m_pGameLogicController->GetCurrentState() == GS_STARTGAME) {
		m_pGameLogicController->pushState(GS_GAMING);
		color = PAWPAWJUICE;
		ParticlesRange = 300;
		NewGame();
	}
	numParticles = rand() % ParticlesRange;
	if (numParticles < 50) numParticles += 50;
	rc = pSprite->GetPosition();
	POINT pt = { rc.left + pSprite->GetWidth() / 2, rc.top + pSprite->GetHeight() / 2 };
	pEngine->GetParticleController()->AddParicles(numParticles, pt, pSprite->GetVelocity(), color);
	velocity.x = velocity.y = 0;
	ParticleTrace *pKnife = new KnifeParticle(pt, velocity, velocity, 6, 250, WHITE);

	pEngine->AddKnife(pKnife);
	PlaySound((LPCWSTR)IDR_WAVE1, pEngine->GetInstance(), SND_ASYNC |
		SND_RESOURCE);
}

void GameEnd() {
	DeleteObject(g_hMemDC);
	DeleteObject(g_hOffscreenBitmap);

	delete g_pStrawbarryBitmap;
	delete g_pAppleBitmap;
	delete g_pWaterMelonBitmap;
	delete g_pBroccoliBitmap;
	delete g_pPeaBitmap;
	delete g_pOnionBitmap;
	delete g_pPimentoBitmap;
	delete g_pEggplantBitmap;
	delete g_pNewGameBitmap;
	delete g_pBomBitmap;
	delete g_pPawpawBitmap;

	delete m_score;
	delete m_time;
	delete m_readytime;

	pEngine->ClearupSprite();
	//g_pGame->ClearupParticles();
	delete pEngine;
	delete m_pGameLogicController;
}

void MainGame() {
	pEngine->ClearupSprite();
	POINT ptpos = { 100, 300 }, ptVelocity = { 0 , 0 };
	RECT rcBounds = { 0,0,800,600 };
	g_pWaterMelonSprite = new Fruit(g_pNewGameBitmap);
	g_pWaterMelonSprite->SetPosition(ptpos);
	g_pWaterMelonSprite->SetNumFrames(24);
	g_pWaterMelonSprite->SetFrameDelay(4);
	//g_pWaterMelonSprite->SetClockDirection(FALSE);
	pEngine->AddSprite(g_pWaterMelonSprite);

	g_pBomSprite = new Fruit(g_pBomBitmap);
	ptpos.x += 200, ptpos.y += 100;
	g_pBomSprite->SetPosition(ptpos);
	g_pBomSprite->SetNumFrames(15);
	g_pBomSprite->SetFrameDelay(4);
	pEngine->AddSprite(g_pBomSprite);
}

void STARTGAME(GAMESTATE st) {
	if (st == GS_MAIN) {
		g_pBomSprite->SetVelocity(0, 10);
		g_pBomSprite->SetBoundsAction(BA_DIE);
	}
	else if (st == GS_GAMING) {
		//pEngine->ClearupSprite();
	}
	m_bitmapNow = m_bitmapStartGameBack;

	POINT pos = { 200 , 100 };
	Sprite *pStartGame = new Fruit(g_pPawpawBitmap);
	pStartGame->SetPosition(pos);
	pEngine->AddSprite(pStartGame);

	gameDelay = 0;
	pEngine->GetMouseBrush()->SetMouseValid(FALSE);
}

void NewGame() {
	stTime = GetTickCount();
	nowScore = 0;
	srand(stTime);
	m_bitmapNow = m_bitmapGaming;
}

Bitmap* GetRandomFruit() {
	int idx = rand() % 9;//0-8
	switch (idx)
	{
	case 0:
		return g_pStrawbarryBitmap;
	case 1:
		return g_pAppleBitmap;
	case 2:
		return g_pWaterMelonBitmap;
	case 3:
		return g_pBroccoliBitmap;
	case 4:
		return g_pPeaBitmap;
	case 5:
		return g_pOnionBitmap;
	case 6:
		return g_pPimentoBitmap;
	case 7:
		return g_pEggplantBitmap;
	default:
		return g_pBomBitmap;
	}
}

Bitmap* GetBomBitmap() {
	return g_pBomBitmap;
}
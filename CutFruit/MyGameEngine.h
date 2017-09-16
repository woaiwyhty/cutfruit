#pragma once

#include "Bitmap.h"
#include "Gamefont.h"
#include "Sprite.h"
#include "Fruit.h"
#include "GameEngine.h"
#include "resource.h"
#include "KnifeParticle.h"
#include "GameLogicController.h"
#pragma comment(lib, "winmm.lib")


void GameStart(HWND m_hWnd);
void GamePaint(HDC hDC);
void GameCycle(HWND m_hWnd);
void MouseCollisionAction(Sprite *pSprite);
void GameEnd();
void MainGame();
void STARTGAME(GAMESTATE st);
void NewGame();
Bitmap* GetRandomFruit();
BOOL ReadHiScores();
BOOL WriteHiScores();

Bitmap* GetBomBitmap();
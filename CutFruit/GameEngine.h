#pragma once

#include "MyGameEngine.h"
#include "Queue.h"
#include "MouseBrush.h"
#include "FruitController.h"
#include "GameLogicController.h"
#include "ParticleController.h"
#include "FontController.h"
#include "ParticleTrace.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class GameEngine {
protected:
	HINSTANCE m_hInst;//应用程序的实例句柄
	HWND m_hWnd;//窗口句柄
	LPTSTR m_szWndTitle;//窗口标题
	int m_cxGame;//游戏画面的宽度
	int m_cyGame;//游戏画面的高度
	int m_iFrameDelay;//帧之间延迟的毫秒数
	BOOL m_bPause;
	HDC m_hdcMem;
	HBITMAP m_hBitmap;
	MouseBrush *m_pMouseBrush;
	SpriteController *m_pSpriteController;
	ParticleController *m_pParticleController;
	FontController *m_pFontController;
	ParticleController *m_pKnifeController;
public:
	GameEngine(LPTSTR szWndTitle, int cx, int xy, int FrameRate);
	virtual ~GameEngine(void);
	HINSTANCE GetInstance() { return m_hInst; }

	int Run(void);
	LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL Init(HINSTANCE hInst);
	//访问内部字段
	HWND GetWnd(void) { return m_hWnd; }
	BOOL GetFramDelay(void) { return m_iFrameDelay; }
	int GetGameWidth(void) { return m_cxGame; }
	int GetGameHeignt(void) { return m_cyGame; }
	BOOL IsPause(void) { return m_bPause; }
	void SetFrameRate(int iFrameRate);
	void AddSprite(Sprite *pSprite);
	void DrawSprites(HDC hDC);
	void AddKnife(ParticleTrace *pKnife) {
		if (pKnife != NULL)
			m_pKnifeController->AddParticle(pKnife);
	}
	void UpdateSprites();
	void ClearupSprite();
	Sprite * IsPointInSprite(int x, int y);
	Sprite * IsPointInSprite(const POINT &point);
	MouseBrush* GetMouseBrush() { return m_pMouseBrush; }
	ParticleController* GetParticleController() { return m_pParticleController; }
	FontController* GetFontController() { return m_pFontController; }
private:
	BOOL CreateWnd(HINSTANCE hInst);
	BOOL CreateBackBuffer(void);
protected:
	 BOOL SetGameParams(void);//初始化角色的参数
	 void PaintToMemDC(void);//重绘帧
	 void ChangeGameParams(void);//改变角色的状态

	 void OnkeyPress(void);//处理按键
	 void OnMouseLButtonDown(int x, int y);//处理鼠标左键按下
	 void OnMouseLButtonUp(int x, int y);//处理鼠标左键释放
	 void OnMouseRButtonDown(int x, int y);//处理鼠标右键按下
	 void OnMouseRButtonUp(int x, int y);//处理鼠标右键释放
	 void OnMouseMove(int x, int y);//处理鼠标移动
};

extern GameEngine *pEngine;
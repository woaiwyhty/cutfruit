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
	HINSTANCE m_hInst;//Ӧ�ó����ʵ�����
	HWND m_hWnd;//���ھ��
	LPTSTR m_szWndTitle;//���ڱ���
	int m_cxGame;//��Ϸ����Ŀ��
	int m_cyGame;//��Ϸ����ĸ߶�
	int m_iFrameDelay;//֮֡���ӳٵĺ�����
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
	//�����ڲ��ֶ�
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
	 BOOL SetGameParams(void);//��ʼ����ɫ�Ĳ���
	 void PaintToMemDC(void);//�ػ�֡
	 void ChangeGameParams(void);//�ı��ɫ��״̬

	 void OnkeyPress(void);//������
	 void OnMouseLButtonDown(int x, int y);//��������������
	 void OnMouseLButtonUp(int x, int y);//�����������ͷ�
	 void OnMouseRButtonDown(int x, int y);//��������Ҽ�����
	 void OnMouseRButtonUp(int x, int y);//��������Ҽ��ͷ�
	 void OnMouseMove(int x, int y);//��������ƶ�
};

extern GameEngine *pEngine;
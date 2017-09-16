#pragma once
#include "Bitmap.h"
#include "Queue.h"

typedef WORD BOUNDSACTION;
const BOUNDSACTION
BA_STOP = 0, BA_WRAP = 1, BA_BOUNCE = 2, BA_DIE = 3, BA_XWRAP_YDIE = 4,
BA_XBOUNCE_YDIE = 5, BA_XTOP_YDIE = 6;

typedef WORD SPRITEACTION;
const SPRITEACTION
SA_NONE = 0x0000L,
SA_KILL = 0x0001L,
SA_ADDSPRITE = 0x0002L;

class Sprite {
public:
	//Constructor and destructor
	Sprite(Bitmap *pBitmap);
	Sprite(Bitmap *pBitmap, RECT &rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap *pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT &rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite(void);

	//----------------------------------------------------------------
	//���淽��
	virtual SPRITEACTION Update();
	virtual Sprite* AddSprite();
	void Draw(HDC hDC);
	BOOL IsPointInside(int x, int y);
	BOOL IsPointInside(const POINT &ptPosition);

	//-------------------------------------------------------------
	//���ʷ���
	const RECT & GetPosition() { return m_rcPosition; };
	void SetPosition(const POINT & ptPosition);
	void SetPosition(int x, int y);
	void SetPosition(const RECT & rcPosition)
	{
		CopyRect(&m_rcPosition, &rcPosition);
	};
	void OffsetPosition(int x, int y);
	const POINT & GetVelocity() { return m_ptVelocity; };
	void SetVelocity(int x, int y);
	void SetVelocity(const POINT & ptVelocity) { m_ptVelocity = ptVelocity; };
	int GetZOrder() { return m_iZOrder; };
	void SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void SetBounds(const RECT & rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL IsHidden() { return m_bHidden; };
	// �����λͼ�Ƿ�����Bound�ı���
	BOOL IsTouchBoundX();
	BOOL IsTouchBoundY();
	void SetHidden(BOOL bHidden) { m_bHidden = bHidden; };
	int GetWidth() { return m_pBitmap->GetWidth(); };
	int GetHeight() { return (m_pBitmap->GetHeight() / m_iNumFrames); };
	Bitmap* GetBitmap();

	void SetBitamp(Bitmap* pBitmap) { m_pBitmap = pBitmap; };

	// Update:2011/7/26
	//--------------------------------------------------------------------------------
	//�����ײ����
	RECT &GetCollision() { return m_rcCollision; };
	//������ײ����
	virtual void CalcCollisionRec();
	virtual BOOL MouseCollision(QUEUE1<POINT> *pMouseBrushData);
	//�ж��Ƿ�����ײ
	BOOL TestCollision(Sprite *pTestSprite);
	//-----------------------------------------------------------------------------------
	// Update:2011/8/13
	void SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE);
	int GetNumFrames() { return m_iNumFrames; };
	void SetFrameDelay(int iFrameDelay);
	void SetCurFrame(int iCurFrame) { m_iCurFrame = iCurFrame; };
	void UpdateFrame();
	//--------------------------------------------------------------------------------
	// Update:2011/8/14-----------------------------------
	void Kill() { m_bDying = TRUE; };
	//-----------------------------------------------------
	// Update : 2012/5/5
	void SetClockDirection(BOOL IsClockWise) { m_bIsClockWise = IsClockWise; };
	BOOL IsClockWise() { return m_bIsClockWise; };
protected:
	Bitmap *m_pBitmap;
	RECT m_rcPosition;
	POINT m_ptVelocity;
	int m_iZOrder;
	RECT m_rcBounds;
	BOUNDSACTION m_baBoundsAction;
	BOOL m_bHidden;

	//Update:2011/7/26
	//---------------------------------------------------
	RECT m_rcCollision;
	//----------------------------------------------------
	// Update:2011/8/13
	int m_iNumFrames;	//֡��
	int m_iCurFrame;	//��ǰ֡
	int m_iFrameDelay;	//֡�ӳ�
	int m_iFrameTrigger;	//֡������
							//----------------------------------------------------
							// Update:2011/8/14----------------------------------
							// ���֡���ź���
	BOOL m_bDying;
	BOOL m_bOneCycle;
	//----------------------------------------------------
	// Update:2012/5/5
	// ��ӿ���֡���ŷ���0-NumFramesΪ˳ʱ�벥�ţ� NumFrames-0Ϊ��ʱ�벥�š�
	// ��Ϊ��ʱ�벥�ŵ�ʱ���������m_iCurFrame=NumFrames-1
	// Ĭ��ֵΪTRUE
	BOOL m_bIsClockWise;

};
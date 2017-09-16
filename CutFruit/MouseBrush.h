#pragma once
#include "queue.h"
class MouseBrush {
public:
	MouseBrush(int sz = 5);
	~MouseBrush();
	virtual void DrawBrush(HDC hDC);
	virtual void Update();
	void InsertPoint(int x, int y);
	void Clear();
	int GetPointSize() { return m_dataPoint->GetSize(); }
	QUEUE1<POINT>* GetData() { return m_dataPoint; }
	void SetMouseValid(BOOL isVal) { m_isVal = isVal; }
	BOOL GetMouseValid() { return m_isVal; }
protected:
	QUEUE1<POINT> *m_dataPoint;
	static const int UPDATE_TIME = 5;
	int m_iTrigger;
	BOOL m_isVal;
};
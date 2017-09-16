#pragma once
#include "stdafx.h"
#include <stack>
typedef WORD GAMESTATE;
const GAMESTATE
GS_MAIN = 0,
GS_STARTGAME = 1,
GS_GAMING = 2,
GS_GAMESTOP = 3;


class GameLogicController {
public:
	static GameLogicController* GetInstance();
	~GameLogicController();
	GAMESTATE GetCurrentState();
	void pushState(GAMESTATE gState);
	void popState();
protected:
	GameLogicController();

	stack<GAMESTATE> m_stack;
	static GameLogicController* m_pGameLogicController;

};
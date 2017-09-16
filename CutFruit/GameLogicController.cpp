#include "GameLogicController.h"

GameLogicController *GameLogicController::m_pGameLogicController = NULL;

GameLogicController* GameLogicController::GetInstance() {
	if (m_pGameLogicController != NULL) return m_pGameLogicController;
	m_pGameLogicController = new GameLogicController;
	return m_pGameLogicController;
}

GameLogicController::GameLogicController() {
	m_stack.push(GS_MAIN);
}

GameLogicController::~GameLogicController() {
	//if (m_pGameLogicController != NULL) delete  m_pGameLogicController;
}

void GameLogicController::pushState(GAMESTATE gState) {
	m_stack.push(gState);
}

void GameLogicController::popState() {
	if(m_stack.size() > 1)
		m_stack.pop();
}

GAMESTATE GameLogicController::GetCurrentState() {
	if (!m_stack.empty()) return m_stack.top();
	return GS_MAIN;
}


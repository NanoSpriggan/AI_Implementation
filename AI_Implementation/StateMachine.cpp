#include "StateMachine.h"
#include "State.h"
#include "Agent.h"

StateMachine::StateMachine() {
	m_pCurrentState = nullptr;
}

StateMachine::~StateMachine() {
	delete m_pCurrentState;
}

void StateMachine::Update(float deltaTime, Agent* pAgent, Player *pPlayer) {
	if (m_pCurrentState == nullptr)
		return;
	m_pCurrentState->Update(deltaTime, pAgent, *pPlayer, this);
}

void StateMachine::ChangeState(Agent* pAgent, State* pNewState) {
	if (m_pCurrentState != nullptr) {
		m_pCurrentState->Exit(pAgent);
		delete m_pCurrentState;
	}
	pNewState->Init(pAgent);
	m_pCurrentState = pNewState;
}
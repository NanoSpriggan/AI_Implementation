#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Player.h"

class State;
class Agent;

class StateMachine {
public:
	StateMachine();
	~StateMachine();
	void Update(float deltaTime, Agent* pAgent, Player *pPlayer);
	void ChangeState(Agent* pAgent, State* pNewState);
private:
	State* m_pCurrentState;
};

#endif // !STATEMACHINE_H
#ifndef STATE_H
#define STATE_H

#include "Player.h"
#include "Agent.h"
#include "StateMachine.h"

class StateMachine;

class State {
protected:
	State() {}
public:
	virtual ~State() {}
	virtual void Init(Agent *pAgent) = 0;
	virtual void Update(float deltaTime, Agent* pAgent, Player pPlayer, StateMachine* pSM) = 0;
	virtual void Exit(Agent *pAgent) = 0;
};

class Seek : public State {
public:
	void Init(Agent*pAgent) {}
	void Update(float deltaTime, Agent* pAgent, Player pPlayer, StateMachine* pSM);
	void Exit(Agent*pAgent) {}
};

class Flee : public State {
public:
	void Init(Agent*pAgent) {}
	void Update(float deltaTime, Agent* pAgent, Player pPlayer, StateMachine* pSM);
	void Exit(Agent*pAgent) {}
};


#endif // !STATE_H
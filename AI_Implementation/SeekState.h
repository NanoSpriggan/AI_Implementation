#ifndef SEEKSTATE_H
#define SEEKSTATE_H
#include "State.h"
#include "Player.h"

class SeekState : public State {
public:
	SeekState() {}
	~SeekState() {}

	void Init(Agent* pAgent) {}
	void Update(float deltaTime, Agent* pAgent, StateMachine* pSM);
	void Exit(Agent *pAgent) {}
};

#endif // !SEEKSTATE_H
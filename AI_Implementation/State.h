#ifndef STATE_H
#define STATE_H

#include "Player.h"
#include "Agent.h"
#include "StateMachine.h"

class StateMachine;

class Seek;
class Flee;

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
	Seek() {}
	~Seek(){}
	void Init(Agent*pAgent) {}
	void Update(float deltaTime, Agent* pAgent, Player pPlayer, StateMachine* pSM) {
		Vector3 diff = Vector3((pPlayer.pos.x - pAgent->pos.x), (pPlayer.pos.y - pAgent->pos.y), 0);
		diff.normalise();
		if (diff.x < 50 || diff.y < 50)
			pSM->ChangeState(pAgent, new Flee()); return;
		Vector3 accel = diff * pAgent->m_movementInfo.m_maxForce;
		pAgent->m_movementInfo.m_acceleration = accel;
	}
	void Exit(Agent*pAgent) {}
};

class Flee : public State {
public:
	Flee() {}
	~Flee() {}
	void Init(Agent*pAgent) {}
	void Update(float deltaTime, Agent* pAgent, Player pPlayer, StateMachine* pSM) {
		Vector3 diff = Vector3((pPlayer.pos.x - pAgent->pos.x), (pPlayer.pos.y - pAgent->pos.y), 0);
		diff.normalise();
		if (diff.x > 500 || diff.y > 500)
			pSM->ChangeState(pAgent, new Seek()); return;
		Vector3 accel = diff * -pAgent->m_movementInfo.m_maxForce;
		pAgent->m_movementInfo.m_acceleration = accel;
	}
	void Exit(Agent*pAgent) {}
};

#endif // !STATE_H
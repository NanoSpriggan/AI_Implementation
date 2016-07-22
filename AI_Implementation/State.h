#ifndef STATE_H
#define STATE_H

class Agent;
class StateMachine;

class State {
protected:
	State() {}
public:
	virtual ~State() {}
	virtual void Init(Agent *pAgent) = 0;
	virtual void Update(float deltaTime, Agent* pAgent, StateMachine* pSM) = 0;
	virtual void Exit(Agent *pAgent) = 0;
};

#endif // !STATE_H
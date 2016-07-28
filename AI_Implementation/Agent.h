#ifndef AGENT_H
#define AGENT_H

#include <Vector3.h>
//#include "Player.h"

class IBehaviour;
class StateMachine;
class Player;

struct MovementInfo {
	Vector3 m_position;
	Vector3 m_velocity;
	Vector3 m_acceleration;

	float m_maxSpeed;
	float m_maxForce;

	float m_rotation;
	float m_rotationDampening;
};

class Agent {
public:
	Agent();
	Agent(Vector3 pos);
	virtual ~Agent();

	inline Vector3 GetPos() { return m_movementInfo.m_position; }
	inline IBehaviour* GetBehaviour() { return m_currentBehaviour; }

	inline void SetBehaviour(IBehaviour *behaviour) { m_currentBehaviour = behaviour; }
	void Update(float deltaTime, Player *pPlayer);
	MovementInfo m_movementInfo;
	Vector3 pos;
protected:
	void Init(Vector3 pos);
	IBehaviour *m_currentBehaviour;
	StateMachine *m_pStateMachine;
};



#endif // !AGENT_H
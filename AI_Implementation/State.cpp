#include "State.h"

void Flee::Update(float deltaTime, Agent * pAgent, Player pPlayer, StateMachine * pSM) {
	Vector3 diff = Vector3((pPlayer.pos.x - pAgent->pos.x), (pPlayer.pos.y - pAgent->pos.y), 0);
	diff.normalise();
	if (diff.x > 500 || diff.y > 500)
		pSM->ChangeState(pAgent, new Seek()); return;
	Vector3 accel = diff * -pAgent->m_movementInfo.m_maxForce;
	pAgent->m_movementInfo.m_acceleration = accel;
}

void Seek::Update(float deltaTime, Agent * pAgent, Player pPlayer, StateMachine * pSM) {
		Vector3 diff = Vector3((pPlayer.pos.x - pAgent->pos.x), (pPlayer.pos.y - pAgent->pos.y), 0);
		diff.normalise();
		if (diff.x < 50 || diff.y < 50)
			pSM->ChangeState(pAgent, new Flee()); return;
		Vector3 accel = diff * pAgent->m_movementInfo.m_maxForce;
		pAgent->m_movementInfo.m_acceleration = accel;
	}

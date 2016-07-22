//#include "SeekState.h"
//#include "StateMachine.h"
//
//class AIObject;
//class Player;
//
//void SeekState::Update(float deltaTime, Agent *pAgent, StateMachine* pSM) {
//	Vector3 diff = pPlayer.pos - pAgent.pos;
//	if (diff.magnitude < 50) {
//		cout << "FLEE";
//		//->ChangeState(pAgent, new FleeState());
//		return;
//	}
//	Vector3 accel = diff.normalise * pAgent.m_movementInfo.m_maxForce;
//	pAgent->m_movementInfo.m_acceleration = accel;
//}
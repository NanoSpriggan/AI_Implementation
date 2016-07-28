#define _USE_MATH_DEFINES
#include <math.h>
#include "Agent.h"
#include "StateMachine.h"
#include "IBehaviour.h"

Agent::Agent() { Init(Vector3(0.0f, 0.0f, 0.0f)); }

Agent::Agent(Vector3 pos) { Init(pos); }

Agent::~Agent(){}

void Agent::Update(float deltaTime, Player *pPlayer) {
	if (m_pStateMachine != nullptr)
		m_pStateMachine->Update(deltaTime, this, pPlayer);
	if (m_currentBehaviour != nullptr)
		m_currentBehaviour->Update(deltaTime, m_movementInfo);
	m_movementInfo.m_velocity += m_movementInfo.m_acceleration * deltaTime;
	if ((m_movementInfo.m_velocity).magnitude() > m_movementInfo.m_maxSpeed) {
		m_movementInfo.m_velocity.normalise();
		m_movementInfo.m_velocity = m_movementInfo.m_velocity * m_movementInfo.m_maxSpeed;
	}
	m_movementInfo.m_rotation = atan2(m_movementInfo.m_velocity.y, m_movementInfo.m_velocity.x) + (float)(M_PI / 2);
	m_movementInfo.m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	m_movementInfo.m_position += m_movementInfo.m_velocity * deltaTime;
}

void Agent::Init(Vector3 pos) {
	m_movementInfo.m_position = pos;
	m_movementInfo.m_velocity = Vector3(0.0f, 0.0f, 0.0f);
	m_movementInfo.m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	m_movementInfo.m_maxSpeed = 200.0f;
	m_movementInfo.m_maxForce = 500;
	m_movementInfo.m_rotation = 0.0f;
	m_movementInfo.m_rotationDampening = 0.05f;
	m_currentBehaviour = nullptr;
	m_pStateMachine = nullptr;
}
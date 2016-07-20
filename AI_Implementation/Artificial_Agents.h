#ifndef _ARTIFICIAL_AGENTS_H_
#define _ARTIFICIAL_AGENTS_H_

#include <Vector2.h>
#include <Vector3.h>
#include <list>
#include <ctime>
#include <chrono>
#include <SDL.h>
#include <map>

typedef uint32_t Uint32;



//class Agent {
//public:
//	void Update() {
//		// TO DO - Sensing and thinking calcultions, also remove and add behaviours from the m_behaviours list
//
//		for (auto iter = m_behaviours.begin(); iter != m_behaviours.end(); iter++)
//			(*iter)->Update(this);
//	}
//protected:
//	std::list<IBehaviour*> m_behaviours;
//	Vector2 m_force, m_acceleration, m_velocity, m_position;
//};
//
//class IBehaviour {
//public:
//	virtual void Update(Agent *pAgent) = 0;
//};
//
//#pragma region Basic Behaviours
//
//class Seek : public IBehaviour {
//public:
//	/*Uint32 previousTime = SDL_GetTicks();
//	Uint32 currentTime = SDL_GetTicks();
//	Uint32 deltaTime = currentTime - previousTime;
//
//	Vector2 *m_targetPos;
//	void Update(Agent *pAgent) {
//		Vector3 dest;
//		float P1, P2;
//		float p12res = P2 - P1;
//		dest.normalise();*/
//};
//
//class Flee : public IBehaviour {
//public:
//
//	virtual void Update(Agent *pAgent);
//};
//
//class Wander : public IBehaviour {
//public:
//	virtual void Update(Agent *pAgent);
//};
//
//#pragma endregion
//
//#pragma region Advanced Behaviours
//class Pursue : public IBehaviour {
//public:
//
//	virtual void Update(Agent *pAgent);
//};
//
//class Evade : public IBehaviour {
//public:
//
//	virtual void Update(Agent *pAgent);
//};
//
//class Arrival : public IBehaviour {
//public:
//
//	virtual void Update(Agent *pAgent);
//};
//#pragma endregion

struct Vector2D {
	double x;
	double y;
	Vector2D() :x(0.0), y(0.0) {}
	Vector2D(double a, double b) :x(a), y(b){}
	inline void Zero();
	inline bool isZero()const;
	inline double Length()const;
	inline double LengthSq()const;
	inline void Normalize();
	inline double Dot(const Vector2D& v2)const;
	inline int Sign(const Vector2D& v2)const;
	inline Vector2D Perp()const;
	inline void Truncate(double max);
	inline double Distance(const Vector2D& v2)const;
	inline double DistanceSq(const Vector2D& v2)const;
	inline Vector2D GetReverse()const;

	const Vector2D& operator+=(const Vector2D &rhs);
	const Vector2D& operator-=(const Vector2D &rhs);
	const Vector2D& operator*=(const Vector2D &rhs);
	const Vector2D& operator/=(const Vector2D &rhs);
	bool operator==(const Vector2D& rhs)const;
	bool operator!=(const Vector2D& rhs)const;
};


template <class entity_type>
class StateMachine {
private:
	entity_type* m_pOwner;
	State<entity_type> m_pCurrentState;
	State<entity_type> m_pPreviousState;
	State<entity_type> m_pGlobalState;
public:
	StateMachine(entity_type* owner) :m_pOwner(owner), m_pCurrentState(NULL), m_pPreviousState(NULL), m_pGlobalState(NULL) {}
	void SetCurrentState(State<entity_type>* s) { m_pCurrentS tate = s; }
	void SetGlobalState(State<entity_type>* s) { m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s) { m_pPreviousState = s; }

	void Update()const {
		if (m_pGlobalState)
			m_pGlobalState->Execute(m_pOwner);
		if (m_pCurrentState)
			m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(State<entity_type>* pNewState) {
		assert(pNewState && "<StateMachine::ChangeState>: trying to change a null state");
		m_pPreviousState = m_pCurrentState;
		m_pCurrentState = Exit(m_pOwner);
		m_pCurrentState = pNewState;
		m_pCurrentState->Enter(m_pOwner);
	}

	void RevertToPreviousState() { ChangeState(m_pPreviousState); }

	State<entity_type>* CurrentState() const { return m_pCurrentState; }
	State<entity_type>* GlobalState() const { return m_pGlobalState; }
	State<entity_type>* PreviousState() const { return m_pPreviousState; }
	bool isInState(const State<entity_type>& st) const;
};

template <class entity_type>
class State {
public:
	virtual ~State() {}
	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;
};

class BaseGameEntity {
private:
	int m_ID;
	static int m_iNextValdID;
	void SetID(int val);
public:
	BaseGameEntity(int id) SetID(id);
	virtual ~BaseGameEntity() {}
	virtual void Update() = 0;
	int ID()const { return m_ID; }
};

class EntityManager {
private:
	typedef std::map<int, BaseGameEntity*> EntityMap;
private:
	EntityMap m_EntityMap;
	EntityManager() {}
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);
public:
	static EntityManager* Instance();
	void RegisterEntity(BaseGameEntity* NewEntity);
	BaseGameEntity* GetEntityFromID(int id)const;
	void RemoveEntity(BaseGameEntity *pEntity);
};

#define EntityMgr EntityManager::Instance()

class AiAgent : public BaseGameEntity {
private:
	StateMachine<AiAgent>* m_pStateMachine;
public:
	AiAgent(int id) :BaseGameEntity(id) {
		m_pStateMachine = new StateMachine<AiAgent>(this);
		m_pStateMachine->SetCurrentState();
		m_pStateMachine->SetGlobalState();
	}
	~AiAgent() { delete m_pStateMachine; }
	
	void Update() { m_pStateMachine->Update(); }
	StateMachine<AiAgent>* GetFSM()const { return m_pStateMachine; }
};

#endif // !_ARTIFICIAL_AGENTS_H_
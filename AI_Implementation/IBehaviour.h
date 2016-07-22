#ifndef IBEHAVIOUR_H
#define IBEHAVIOUR_H

struct MovementInfo;

class IBehaviour {
public:
	IBehaviour() {}
	virtual ~IBehaviour() = 0 {}
	virtual void Update(float deltaTime, MovementInfo& behaviour) = 0;
};

#endif // !IBEHAVIOUR_H
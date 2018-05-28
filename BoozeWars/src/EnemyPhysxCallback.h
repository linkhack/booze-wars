#pragma once
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;
class EnemyPhysxCallback : PxSimulationEventCallback
{
public:
	EnemyPhysxCallback();
	~EnemyPhysxCallback();
};


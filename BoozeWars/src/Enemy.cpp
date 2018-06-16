#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Street> street)
{
	this->model = model;
	glm::mat2 part = street->getPart1();
	x = part[0][0];
	z = 0;
	y = part[0][1]+ 0.5*(street->getStreetWidth());
	this->street = street;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::DEFAULT_ENEMY);
	movementspeed = 20.0f;
	hp = 100;
	damageTeens = 5;
	streetPart = 0;
	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(8, 8, 8));
	this->width = 1.0f;
	this->width = 1.0f;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, z-1, y))*modelMatrix;
}
void Enemy::drawShadows(Shader& shader)
{
	//model->drawShadow(shader, getModelMatrix());
}

void Enemy::draw(Shader* shader) 
{
	model->draw(shader, getModelMatrix());
}

float Enemy::getX() 
{
	return x;
}

float Enemy::getZ() 
{
	return y;
}

float Enemy::getHP()
{
	return hp;
}
/*
	Returns damage given
*/
float Enemy::getDamage() {
	return damageTeens;
}
float Enemy::getDistanceSquared(const Enemy& otherEnemy)
{
	return (x-otherEnemy.x)*(x - otherEnemy.x)+ (y - otherEnemy.y)*(y - otherEnemy.y)+ (z - otherEnemy.z)*(z - otherEnemy.z);
}
PxVec3 Enemy::getDesiredDirection()
{
	PxVec3 goal;
	PxVec3 goalDirection;

	//calculation of goal
	float goalx;
	float goaly;
	if (x < street->getPart1()[1][0] && streetPart == 0) {
		goalx = street->getPart1()[1][0] ;
		goaly = (street->getPart1()[0][1] + street->getPart1()[1][1])*0.5;

		streetPart = 0;
		
	}
	else if (y < street->getPart2()[1][1] - street->getStreetWidth() && (streetPart == 0 || streetPart == 1)) {
		if (streetPart == 0) {
			//physxActor->addForce(PxVec3(-20.0f, 20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		goaly = street->getPart2()[1][1];
		goalx = (street->getPart2()[0][0] + street->getPart2()[1][0])*0.5;
		streetPart = 1;
	}
	else {
		if (streetPart == 1) {
			//physxActor->addForce(PxVec3(20.0f, -20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		goalx = street->getPart3()[1][0] + street->getStreetWidth()*0.5;
		goaly = (street->getPart3()[0][1] + street->getPart3()[1][1])*0.5;
		streetPart = 2;
	}
	
	goal = PxVec3(goalx, goaly, 0.0f);

	//calculation of direction goal - position normalized
	goalDirection = goal - PxVec3(x,y,0);
	return goalDirection.getNormalized();

}

PxVec3 Enemy::getDirectionVector(const Enemy & otherEnemy)
{
	return PxVec3(otherEnemy.x-x,otherEnemy.y-y,otherEnemy.z-z);
}

void Enemy::hit(float damage)
{	
	hp -= damage;
}

void Enemy::selfDestruct()
{
	physxActor->release();
}

PxRigidDynamic* Enemy::createPhysics(PxPhysics* physicsSDK)
{
	PxMaterial* enemyMaterial;
	enemyMaterial = physicsSDK->createMaterial(0.0f, 0.0f, 0.0f);
	physxActor = PxCreateDynamic(*physicsSDK, PxTransform(PxVec3(x, y, 3.0f+0.001f)), PxBoxGeometry(2, 2, 3), *enemyMaterial,0.1f);
	//physxActor->setLinearVelocity(PxVec3(20.0f, 0.0f, 0.0f));
	physxActor->setMaxAngularVelocity(0.0f);
	
	return physxActor;
}

void Enemy::applyForce(PxVec3 force)
{
	physxActor->addForce(force);
}

void Enemy::applyDrivingForce()
{
	PxVec3 velocity = physxActor->getLinearVelocity();

	PxVec3 acceleration = 1.7*(movementspeed * getDesiredDirection() - velocity);

	physxActor->addForce(acceleration*physxActor->getMass());
}

void Enemy::updatePosition()
{	
	PxVec3 transform = physxActor->getGlobalPose().p;
	x = transform.x;
	y = transform.y;
	z = transform.z;
	/*
	if (x < street->getPart1()[1][0] + 0.5*street->getStreetWidth()) {
		x += movementspeed * dT;
	}
	else if (y < street->getPart2()[1][1] - 0.6*street->tStreetWidth()) {
		y += movementspeed * dT;
	}
	else {
		x += movementspeed * dT;
	}
	*/
}


glm::mat2x2 Enemy::getModel() 
{
	return glm::mat2x2(x, y, x+width, y+length);
}
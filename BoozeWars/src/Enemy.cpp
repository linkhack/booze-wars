#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Street> street, const int modelNr)
{
	this->model = model;
	glm::mat2 part = street->getPart1();
	x = part[0][0];
	z = 0;
	y = part[0][1]+ 0.5*(street->getStreetWidth());
	this->street = street;
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(modelNr);
	movementspeed = 20.0f;
	hp = 100;
	damageTeens = 5;
	streetPart = 0;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, -z, y))*glm::scale(glm::mat4(1.0f), glm::vec3(10, -10, 10));
}
void Enemy::drawShadows(Shader& shader)
{
	//model->drawShadow(shader, getModelMatrix());
}

void Enemy::draw(Shader* shader) {

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
	PxVec3 direction;
	PxVec3 velocity = physxActor->getLinearVelocity();

	if (x < street->getPart1()[1][0]&&streetPart==0) {

		streetPart = 0;
	}
	else if (y < street->getPart2()[1][1] - street->getStreetWidth() && (streetPart==0||streetPart==1)) {
		if(streetPart==0){
			physxActor->addForce(PxVec3(-20.0f, 20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		streetPart = 1;
	}
	else {
		if (streetPart == 1) {
			physxActor->addForce(PxVec3(20.0f, -20.0f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
		}
		streetPart = 2;
	}
	switch (streetPart)
	{
	case 0:
		direction = PxVec3(1.0f,-velocity.y*0.5f + (street->getPart1()[0][1] + 0.5*street->getStreetWidth()) - y, 0.0f);
		break;
	case 1:
		direction = PxVec3(-velocity.x*0.5f + (street->getPart2()[0][0] + 0.5*street->getStreetWidth()) - x, 1.0f, 0.0f);
		break;
	case 2:
		direction = PxVec3(1.0f, -velocity.y*0.5f + (street->getPart3()[0][1] + 0.5*street->getStreetWidth()) - y, 0.0f);
		break;
	default:
		break;
	}
	return direction;
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
	enemyMaterial = physicsSDK->createMaterial(0.5f, 0.5f, 1.0f);
	physxActor = PxCreateDynamic(*physicsSDK, PxTransform(PxVec3(x, y, z+0.001f)), PxBoxGeometry(2, 2, 3), *enemyMaterial,1.0f);
	physxActor->setLinearVelocity(PxVec3(20.0f, 0.0f, 0.0f));
	physxActor->setMaxAngularVelocity(0.0f);
	
	return physxActor;
}

void Enemy::applyForce(PxVec3 force)
{
	physxActor->addForce(1000.0f*force);
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
	else if (y < street->getPart2()[1][1] - 0.6*street->getStreetWidth()) {
		y += movementspeed * dT;
	}
	else {
		x += movementspeed * dT;
	}
	*/
}
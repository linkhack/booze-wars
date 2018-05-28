#include "Enemy.h"

Enemy::Enemy() 
{

}

Enemy::Enemy(std::shared_ptr<Street> street, std::shared_ptr<Geometry> model)
{
	this->model = model;
	glm::mat2 part = street->getPart1();
	x = part[0][0];
	z = 3;
	y = part[0][1]+ 0.5*(street->getStreetWidth());
	this->street = street;
	movementspeed = 20.0f;
	hp = 100;
	damageTeens = 5;
}



Enemy::~Enemy()
{
}

glm::mat4 Enemy::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, -z, y));
}

void Enemy::drawShadows(Shader & shader)
{
	model->drawShadow(shader, getModelMatrix());
}

void Enemy::draw() {

	model->draw(getModelMatrix());
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
float Enemy::getDistanceSquared(Enemy & otherEnemy)
{
	return (x-otherEnemy.x)*(x - otherEnemy.x)+ (y - otherEnemy.y)*(y - otherEnemy.y)+ (z - otherEnemy.z)*(z - otherEnemy.z);
}
PxVec3 Enemy::getDesiredDirection()
{
	PxVec3 direction;

	if (x < street->getPart1()[1][0] + 0.5*street->getStreetWidth()) {

		direction = PxVec3(1000.0f, 0, 0);
	}
	else if (y < street->getPart2()[1][1] - 0.6*street->getStreetWidth()) {
		direction = PxVec3(0.0f, 10.0f, 0.0f);
	}
	else {
		direction = PxVec3(10.0f, 0.0f, 0.0f);
	}
	return direction;
}
void Enemy::hit(float damage)
{	
	hp -= damage;
}

void Enemy::selfDestruct()
{
}

PxRigidDynamic* Enemy::createPhysics(PxPhysics* physicsSDK)
{
	PxMaterial* enemyMaterial;
	enemyMaterial = physicsSDK->createMaterial(0.0f, 0.0f, 1.0f);
	physxActor = PxCreateDynamic(*physicsSDK, PxTransform(PxVec3(x, y, z+0.001f)), PxBoxGeometry(3, 3, 3), *enemyMaterial,1.0f);
	physxActor->setLinearVelocity(PxVec3(20.0f, 0.0f, 0.0f));
	physxActor->setMaxAngularVelocity(0.0f);
	
	return physxActor;
}

void Enemy::applyForce(PxVec3 force)
{
	physxActor->addForce(force);
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
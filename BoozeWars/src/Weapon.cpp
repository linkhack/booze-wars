#include "Weapon.h"

#include "Building.h"

Weapon::Weapon(float x, float z, int direction, int delay)
{
	ModelFactory* factory = ModelFactory::Instance();
	model = factory->getModel(ModelFactory::DEFAULT_WEAPON);
	glm::mat4 position;
	float startpoint;
	switch (delay) {
	case Weapon::DELAY_FIRST:
		startpoint = Building::width / 2;
		break;
	case Weapon::DELAY_SECOND:
		startpoint = Building::width / 4;
		break;
	case Weapon::DELAY_THIRD:
		startpoint = 0.0;
		break;
	case Weapon::DELAY_FOURTH:
		startpoint = -Building::width / 4;
		break;
	case Weapon::DELAY_FIFTH:
		startpoint = -Building::width / 2;
		break;
	}
	this->direction = direction;
	switch (direction) {
	case 0:
		this->x = x - startpoint;
		this->z = z + Building::length / 2;
		break;
	case 1:
		this->x = x + Building::length / 2;
		this->z = z - startpoint;
		break;
	case 2:
		this->x = x - startpoint;
		this->z = z - Building::length / 2;
		break;
	default:
		this->x = x - Building::length / 2;
		this->z = z - startpoint;
		break;
	}
	this->y = 2.0f;
	this->modelMatrix = glm::mat4(1.0f);
	time = 0.0;
	ttl = 3.0;
	this->delay = (float) delay/10;
	this->implodeTime = 0.0;
	this->width = 1.0f;
	this->length = 2.0f;
}

Weapon::~Weapon()
{
}

bool Weapon::draw(Shader* shader, float time)
{
	this->time += time;
	if (this->time >= this->delay) {
		glm::mat4 rotation;
		if (direction == 1 || direction == 3) {
			rotation = glm::rotate(glm::mat4(1.0f), 1 * glm::pi<float>() / 2, glm::vec3(1, 0, 0));
		}
		else {
			rotation = glm::rotate(glm::mat4(1.0f), 1 * glm::pi<float>() / 2, glm::vec3(0, 0, 1));
		}
		rollRotate += 10.0;
		glm::mat4 roll = glm::rotate(glm::mat4(1.0f), rollRotate * time * glm::pi<float>() / 2, glm::vec3(0, 1, 0));
		model->draw(shader, getModelMatrix() * rotation * roll);

		if (!hitted) {
			glm::mat4 translate;

			switch (this->direction) {
			case 0:
				this->z += movementspeed * time;
				break;
			case 1:
				this->x += movementspeed * time;
				break;
			case 2:
				this->z -= movementspeed * time;
				break;
			default:
				this->x -= movementspeed * time;
				break;
			}
		}
		return this->time >= this->ttl + this->delay;
	}
	return false;
}

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

bool Weapon::implode(float time)
{
	implodeTime += time;

	//TODO: implode particel system
	float implodeScale = 15.0f*time;
	if (implodeScale >=1) {
		implodeScale = 0.2f;
	}
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(implodeScale, implodeScale, implodeScale));
	std::cout << this->time << std::endl;
	this->modelMatrix = this->modelMatrix * scale;
	if (implodeTime >= 3.0) {
		return true;
	}
	return false;
}

glm::mat4 Weapon::getModelMatrix()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z-1))*modelMatrix;
}

glm::mat2x2 Weapon::getModel()
{
	return glm::mat2x2(x, z, x + width, z + length);
}

void Weapon::hit()
{
	this->hitted = true;
}

bool Weapon::isHitted()
{
	return hitted;
}
#include "Hand.h"



Hand::Hand()
{
	this->rotation = 2.5;
	this->up = this->rotation < 0;
	this->model = ModelFactory::Instance()->getModel(ModelFactory::HAND);
	this->modelMatrix = glm::rotate(glm::mat4(1.0f), glm::pi<float>()-2.0f, glm::vec3(0, 1.0, 0)) * 
		glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.8f, 0.9f)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(0.002, 0.002, 0.002));
}


Hand::~Hand()
{
}

void Hand::draw(Shader* shader, glm::mat4 modelMatrix)
{
	glm::mat4 modifiedMatrix = modelMatrix * this->modelMatrix * 
		glm::rotate(glm::mat4(1.0f), this->rotation * glm::pi<float>() /20, glm::vec3(1.0, 0, 0)) * 
		glm::translate(glm::mat4(1.0f), glm::vec3(0, -70 * rotation, 40 * rotation));
	this->model->draw(shader, modifiedMatrix);
}

void Hand::rotate(float time)
{
	if (this->rotation <= -2.5) {
		this->up = true;
	}
	if (this->rotation >= 2.5) {
		this->up = false;
	}
	if (this->up) {
		this->rotation += 5 * time;
	}
	else {
		this->rotation -= 5 * time;
	}
}
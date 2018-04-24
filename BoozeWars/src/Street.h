#pragma once
#include <glm\glm.hpp>

class Street
{
private:
	glm::mat2x2 part1;
	glm::mat2x2 part2;
	glm::mat2x2 part3;
	float streetWidth;
	
public:
	Street();
	~Street();
	Street(float x, float y);

	glm::mat2x2 getPart1();
	glm::mat2x2 getPart2();
	glm::mat2x2 getPart3();
	float getStreetWidth();

};


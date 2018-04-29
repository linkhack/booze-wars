#include "Street.h"



Street::Street()
{
}

Street::Street(float x, float z)
{
	streetWidth = 0.1*z;
	part1 = glm::mat2x2(0.0f, 0.2*z, (x - streetWidth) / 2, 0.2*z + streetWidth);
	part2 = glm::mat2x2((x - streetWidth) / 2, 0.2*z, (x - streetWidth) / 2 + streetWidth, 0.8*z);
	part3 = glm::mat2x2((x - streetWidth) / 2 + streetWidth, 0.7 * z, x, 0.8*z);
}

Street::~Street()
{
}

glm::mat2x2 Street::getPart1()
{
	return part1;
}

glm::mat2x2 Street::getPart2()
{
	return part2;
}

glm::mat2x2 Street::getPart3()
{
	return part3;
}

float Street::getStreetWidth() 
{
	return streetWidth;
}
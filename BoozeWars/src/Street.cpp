#include "Street.h"



Street::Street()
{
}

Street::Street(float x, float y)
{
	float streetWidth = 0.1*y;
	part1 = Segment();
	part1.start = glm::vec2(0, 0.2*y);
	part1.end = glm::vec2((x - streetWidth) / 2, 0.2*y + streetWidth);
	part2.start = glm::vec2((x - streetWidth) / 2, 0.2*y);
	part2.end = glm::vec2((x - streetWidth) / 2 + streetWidth, 0.8*y);
	part3.start = glm::vec2((x - streetWidth) / 2 + streetWidth, 0.7 * y);
	part3.end = glm::vec2(x, 0.8*y);
}

Street::~Street()
{
}

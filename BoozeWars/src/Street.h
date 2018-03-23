#pragma once
#include <glm\glm.hpp>
struct Segment {
	glm::vec2 start;
	glm::vec2 end;
};
class Street
{
private:
	Segment part1;
	Segment part2;
	Segment part3;

public:
	Street();
	~Street();
	Street(float x, float y);
};


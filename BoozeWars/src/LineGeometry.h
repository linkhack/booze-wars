#pragma once
#include "Geometry.h"
class LineGeometry:	public Geometry
{
public:
	LineGeometry(glm::mat4 modelMatrix, GeometryData& data, std::shared_ptr<Material> material);
	LineGeometry(glm::mat4 modelMatrix);
	~LineGeometry();

	LineGeometry(LineGeometry&&) = default;
	LineGeometry& operator=(LineGeometry&&) = default;

	virtual void draw(glm::mat4 matrix = glm::mat4(1.0f));
	static GeometryData createCircle(unsigned int segments, float radius);
};


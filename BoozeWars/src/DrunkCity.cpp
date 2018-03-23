#include "DrunkCity.h"


DrunkCity::DrunkCity()
{

}

DrunkCity::DrunkCity(float x, float y, float z, std::shared_ptr<Material> material)
{
	worldModel = Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(x, y, z), material);
	highway = Street(x, y);
}

DrunkCity::~DrunkCity()
{
}

void DrunkCity::draw() 
{
	//worldModel.draw();
}
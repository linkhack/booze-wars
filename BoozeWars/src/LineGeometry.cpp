#include "LineGeometry.h"



LineGeometry::LineGeometry(glm::mat4 modelMatrix, GeometryData& data, std::shared_ptr<Material> material)
	: Geometry(modelMatrix, data, material)
{
}

LineGeometry::LineGeometry(glm::mat4 modelMatrix)
	: Geometry(modelMatrix)
{
}

LineGeometry::~LineGeometry()
{
}


//Methods

void LineGeometry::draw(glm::mat4 matrix)
{
	glm::mat4 accumModel = matrix * _transformMatrix * _modelMatrix;
	if (!_isEmpty) {
		Shader* shader = _material->getShader();
		shader->use();

		shader->setUniform("modelMatrix", accumModel);
		shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(accumModel))));
		_material->setUniforms();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _elements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	for (size_t i = 0; i < _children.size(); i++) {
		_children[i]->draw(accumModel);
	}
}

GeometryData LineGeometry::createCircle(unsigned int segments, float radius)
{
	GeometryData data;
	float angle_step = 2.0f * glm::pi<float>() / float(segments);
	for (unsigned int i = 0; i < segments; i++)
	{
		glm::vec3 circlePos = glm::vec3(
			glm::cos(i * angle_step) * radius,
			0,
			glm::sin(i * angle_step) * radius
		);
		glm::vec2 squareToCirlceUV = glm::vec2(
			(circlePos.x / radius) * 0.5f + 0.5f,
			(circlePos.z / radius) * 0.5f + 0.5f
		);
		data.positions.push_back(glm::vec4(circlePos, 1));
		data.normals.push_back(glm::vec3(0, -1, 0));
		data.uvs.push_back(squareToCirlceUV);
		data.indices.push_back(i);
		data.indices.push_back((i+1)%(segments));
		data.indices.push_back((i + 2) % (segments));
	}
	return std::move(data);
}
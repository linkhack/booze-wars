#pragma once

#include <sstream>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

class Character
{
private:
	FT_Face face;
	std::shared_ptr<Shader> shader;
	void renderText(const char *text, float x, float y, float sx, float sy);
public:
	Character(FT_Face face);
	~Character();

	void display(char* text, GLFWwindow *window);
};


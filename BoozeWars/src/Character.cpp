#include "Character.h"

Character::Character(GLFWwindow *window)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vboPositions);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//bin vertex positions to location 0

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, "C://Windows//Fonts//arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//shader
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	shader = std::make_shared<Shader>("character.vert", "character.frag");
}

Character::~Character()
{
}

void Character::renderText(const char *text, float x, float y, GLfloat scale, glm::vec3 color) {
	shader->use();
	glUniform3f(glGetUniformLocation(shader->getID(), "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_vao);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	const char *p;
	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
		std::map<char, GLuint>::iterator it;
		it = charToTexture.find(*p);
		if (it==charToTexture.end()) {
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			charToTexture.insert(std::pair<char,GLuint>(*p, textureID));
		}
		else {
			GLuint textureID = it->second;
			glBindTexture(GL_TEXTURE_2D, textureID);
		}


		glUniform1i(glGetUniformLocation(shader->getID(), "tex"), 0);

		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
		glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, false, glm::value_ptr(projection));

		float xpos = x + (face->glyph->bitmap_left * scale);
		float ypos = y - (face->glyph->bitmap_top - face->glyph->bitmap.rows) * scale;
		float w = face->glyph->bitmap.width * scale;
		float h = face->glyph->bitmap.rows * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (face->glyph->advance.x >> 6) * scale;
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Character::setFontSize(int fontSize)
{
	FT_Set_Pixel_Sizes(face, 0, fontSize);
}

void Character::renderBox(glm::vec2 triangle, float x, float y, GLfloat scale, glm::vec3 color)
{
	shader->use();
	glUniform3f(glGetUniformLocation(shader->getID(), "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_vao);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		triangle[0] * scale,
		triangle[1] * scale,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(shader->getID(), "tex"), 0);

	glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, false, glm::value_ptr(projection));

	float xpos = x;
	float ypos = y;
	float w = triangle[0] * scale;
	float h = triangle[1] * scale;

	GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos,     ypos,       0.0, 1.0 },
	{ xpos + w, ypos,       1.0, 1.0 },
	{ xpos,     ypos + h,   0.0, 0.0 },
	{ xpos + w, ypos,       1.0, 1.0 },
	{ xpos + w, ypos + h,   1.0, 0.0 }
	};
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
}
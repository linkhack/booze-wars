#include "Character.h"

Character::Character(FT_Face face)
{
	shader = std::make_shared<Shader>("character.vert", "character.frag");
	this->face = face;
}

Character::~Character()
{
}

void Character::renderText(const char *text, float x, float y, float sx, float sy) {
	const char *p;

	FT_GlyphSlot g = face->glyph;

	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2,     -y2    , 0, 0 },
			{ x2 + w, -y2    , 1, 0 },
			{ x2,     -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x / 64) * sx;
		y += (g->advance.y / 64) * sy;
	}
}

void Character::display(char* text, GLFWwindow *window) {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::vec4 red = glm::vec4( 1, 0, 0, 1 );
	shader->setUniform("color", red);

	int windowWidth;
	int windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	float sx = 2.0 / windowWidth;
	float sy = 2.0 / windowHeight;

	renderText(text, -1 + 8 * sx, 1 - 50 * sy, sx, sy);

	shader->use();

	//glfwSwapBuffers();
}
#include "Character.h"

Character::Character()
{

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	//Generate VBO

	glGenBuffers(1, &_vboPositions);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, "C://Windows//Fonts//arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//shader
	shader = std::make_shared<Shader>("character.vert", "character.frag");
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
		std::map<char, GLuint>::iterator it;
		it = charToTexture.find(*p);
		if (it==charToTexture.end()) {
			GLuint textureID;
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &textureID);

			glBindTexture(GL_TEXTURE_2D, textureID);
			//shader->setUniform("tex", 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
			charToTexture.insert(std::pair<char,GLuint>(*p, textureID));
		}
		else {
			GLuint textureID = it->second;
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		
		

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

		glBufferData(GL_ARRAY_BUFFER, 16*sizeof GLfloat, box, GL_DYNAMIC_DRAW);

		//bin vertex positions to location 0

		//glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x / 64) * sx;
		y += (g->advance.y / 64) * sy;
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Character::display(char* text, GLFWwindow *window) {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glm::vec4 red = glm::vec4( 1, 0, 0, 1 );
	//shader->setUniform("color", red);

	int windowWidth;
	int windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	float sx = 2.0 / windowWidth;
	float sy = 2.0 / windowHeight;

	renderText(text, -1 + 8 * sx, 1 - 50 * sy, sx, sy);

	shader->use();

	//glfwSwapBuffers(window);
}

void Character::setFontSize(int fontSize)
{
	FT_Set_Pixel_Sizes(face, 0, fontSize);
}
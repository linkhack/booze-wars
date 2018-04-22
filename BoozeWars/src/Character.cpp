#include "Character.h"

Character::Character(GLFWwindow *window)
{
	GLfloat box[4][4] = {
		{0,     0   , 0, 0 },
	{ 1, 0    , 1, 0 },
	{ 0,    1, 0, 1 },
	{ 1, 1, 1, 1 },
	};


	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	//Generate VBO
	glGenBuffers(1, &_vboPositions);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);

	//bin vertex positions to location 0

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(ft, "C://Windows//Fonts//arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//shader
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
	shader = std::make_shared<Shader>("character.vert", "character.frag");
	shader->setUniform(glGetUniformLocation(shader->getID(), "projection"), projection);
	GLfloat[4][4] proj= projection;
	glUniformMatrix4fv(,1,false, projection);
}

Character::~Character()
{
}

void Character::renderText(const char *text, float x, float y) {
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

		shader->use();

		GLfloat red[4] = { 1, 0, 0, 1 };
		glUniform1i(glGetUniformLocation(shader->getID(), "tex"), 0);
		glUniform4fv(glGetUniformLocation(shader->getID(), "color"), 1, red);
		

		float x2 = x + g->bitmap_left ;
		float y2 = y - g->bitmap_top ;
		float w = g->bitmap.width ;
		float h = g->bitmap.rows;

		GLfloat box[4][4] = {
			{ x2,     y2    , 0, 0 },
			{ x2 + w, y2    , 1, 0 },
			{ x2,     y2 + h, 0, 1 },
			{ x2 + w, y2 + h, 1, 1 },
		};
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Character::display(char* text) {
	//glClearColor(1, 1, 1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	//glm::vec4 red = glm::vec4( 1, 0, 0, 1 );
	//shader->setUniform("color", red);

	renderText(text, 10, 10);

	//glfwSwapBuffers(window);
}

void Character::setFontSize(int fontSize)
{
	FT_Set_Pixel_Sizes(face, 0, fontSize);
}
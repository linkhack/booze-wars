/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/


#include "Utils.h"

#include <sstream>

#include "Geometry.h"

#include "myCamera.h"
#include "Shader.h"

#include "Material.h"
#include "Light.h"

#include "Texture.h"

#include "DrunkCity.h"

#include "Exceptions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void setPerFrameUniforms(Shader* shader, myCamera& camera, DirectionalLight& dirL, PointLight& pointL);
void setPerFrameUniformsSkybox(Shader* shader, myCamera& camera);


/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

static bool _wireframe = false;
static bool _culling = true;
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 12.0f;
static int _key_pressed = GLFW_KEY_UNKNOWN;
unsigned char **tex;
static int width = 800;
static int height = 800;


/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	INIReader reader("assets/settings.ini");

	if (reader.ParseError() < 0) {
		EXIT_WITH_ERROR("Failed to load 'settings.ini'")
	}

	int window_width = reader.GetInteger("window", "width", width);
	int window_height = reader.GetInteger("window", "height", height);
	std::string window_title = reader.Get("window", "title", "ECG");
	float fov = float(reader.GetReal("camera", "fov", 60.0f));
	float nearZ = float(reader.GetReal("camera", "near", 0.1f));
	float farZ = float(reader.GetReal("camera", "far", 1000.0f));

	/* --------------------------------------------- */
	// Create context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Request OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Request core profile													  
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  // Create an OpenGL debug context 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent window resizing because viewport would have to resize as well (-> not needed in this course)
	
	// Open window
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(window);

	//Delegate Cursor calculations to glfw
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true;
	GLenum err = glewInit();

	// If GLEW wasn't initialized
	if (err != GLEW_OK) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to init GLEW")
	}

#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif


	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */
	if (!initFramework()) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to init framework")
	}

	// set callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// set some GL defaults
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set up TrueType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "C://Windows//Fonts//arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters;

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
		//Shaders
		std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("texture.vert", "texture.frag");
		std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("skybox.vert", "skybox.frag");
		std::shared_ptr<Shader> infiniGreen = std::make_shared<Shader>("texture.vert", "infiniGreen.frag");
		std::shared_ptr<Shader> translucent = std::make_shared<Shader>("texture.vert", "translucentRed.frag");
		//Textures
		std::shared_ptr<Texture> sunTexture = std::make_shared<Texture>("sun.dds");
		std::shared_ptr<Texture> moonTexture = std::make_shared<Texture>("moon.dds");
		std::shared_ptr<Texture> earthTexture = std::make_shared<Texture>("earth.dds");
		std::shared_ptr<Texture> brickTexture = std::make_shared<Texture>("bricks_diffuse.dds");
		std::shared_ptr<Texture> mapTexture = std::make_shared<Texture>("map.dds");

		// Create materials
		std::shared_ptr<Material> sunMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, sunTexture);
		std::shared_ptr<Material> earthMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.1f), 5.0f, earthTexture);
		std::shared_ptr<Material> moonMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.05f), 2.0f, moonTexture);
		std::shared_ptr<Material> infiniGreenMat = std::make_shared<Material>(infiniGreen, glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);
		std::shared_ptr<Material> translucentRed = std::make_shared<Material>(translucent, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
		std::shared_ptr<Material> mapMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, mapTexture);
		//Create World
		DrunkCity world = DrunkCity(20000.0f, 9000.0f, 5000.0f);
		//Create Ground
		Geometry ground = Geometry(glm::mat4(1.0f), Geometry::createInfinitePlane(), infiniGreenMat);
		//Create map
		Geometry map = Geometry(glm::mat4(1.0f), Geometry::createRectangle(1000, 1000), mapMaterial);
		//Helper Rectangle for building placment
		Geometry cameraPlacement = Geometry(glm::mat4(1.0f), Geometry::createRectangle(10.0f, 10.0f), translucentRed);
		//Skybox
		std::vector<std::string> skymapTextureLoc =
		{
			"assets/textures/cubemap/posx.png",
			"assets/textures/cubemap/negx.png",
			"assets/textures/cubemap/negy.png",
			"assets/textures/cubemap/posy.png",
			"assets/textures/cubemap/negz.png",
			"assets/textures/cubemap/posz.png"
		};
		Skybox worldModel = Skybox(skyboxShader, skymapTextureLoc);
		//create enemy
		world.addEnemy(earthMaterial);
		int lookingx;
		int lookingy;
		//create building
		Building* buildingInHand = new Building(4, 5, moonMaterial);

		//Geometry worldModel = Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(x, y, z), material);
		// Initialize camera
		myCamera camera(fov, float(window_width) / float(window_height), nearZ, farZ);
		PointLight pointL(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(1, 0, 0));
		DirectionalLight dirL(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(0,1.0f,0));


		// Render loop
		double mouse_x, mouse_y;
		float t = float(glfwGetTime());
		float dt = 0.0f;
		float t_sum = 0.0f;
		bool firstRun = true;
		while (!glfwWindowShouldClose(window)) {
			// Clear backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Update camera
			glfwGetCursorPos(window, &mouse_x, &mouse_y);

			camera.updatePosition(_key_pressed);
			camera.updateDirection(int(mouse_x), int(mouse_y));

			// Set per-frame uniforms
			setPerFrameUniforms(textureShader.get(), camera, dirL, pointL);
			setPerFrameUniforms(infiniGreen.get(), camera, dirL, pointL);
			setPerFrameUniforms(translucent.get(), camera, dirL, pointL);
			setPerFrameUniformsSkybox(skyboxShader.get(), camera);

			// Hierarchical animation

			// Placing logic
			if (_dragging) {
				try {
					// trying to place building
					world.placeBuilding(camera.getPosition()[0], camera.getPosition()[1], buildingInHand);
					// if placing was successful, create a new building to be placed
					buildingInHand = new Building(4, 5, moonMaterial);
				}
				catch (int e) {
					if (e == PLACING_COLLISION) {
						//something happens
					}
				}
			}
			
			// Play logic
			if (firstRun) {
				firstRun = false;
			}
			else {
				try {
					world.fight();
				}
				catch (int e) {
					if (e == ALL_ENEMIES_DESTROYED) {
						glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

						for (GLubyte c = 0; c < 128; c++)
						{
							// Load character glyph 
							if (FT_Load_Char(face, c, FT_LOAD_RENDER))
							{
								std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
								continue;
							}
							// Generate texture
							GLuint texture;
							glGenTextures(1, &texture);
							glBindTexture(GL_TEXTURE_2D, texture);
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
							// Set texture options
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							// Now store character for later use
							Character character = {
								texture,
								glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
								glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
								face->glyph->advance.x
							};
							Characters.insert(std::pair<GLchar, Character>(c, character));
						}
					}

					if (e == NO_ENEMIES_IN_RANGE) {
						// DO NOTHING
					}
				}
			}

			// Render
			//ground.draw();

			//world.zeichne();
			map.draw();
			if (camera.getGroundIntersection() != glm::vec3(0, 1, 0)) {
				cameraPlacement.draw(glm::translate(glm::mat4(1.0f), camera.getGroundIntersection()+glm::vec3(0.0f,-0.01f,0.0f)));
			}
			//worldModel.draw();
			
			// Compute frame time
			dt = t;
			t = float(glfwGetTime());
			dt = t - dt;
			t_sum += dt;

			// Poll events and swap buffers
			glfwPollEvents();
			glfwSwapBuffers(window);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "GL Error detected" << std::endl;
			}
		}
	}


	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	destroyFramework();


	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */

	glfwTerminate();

	return EXIT_SUCCESS;
}


void setPerFrameUniforms(Shader* shader, myCamera& camera, DirectionalLight& dirL, PointLight& pointL)
{
	shader->use();
	shader->setUniform("viewProjMatrix", camera.getViewProjectionMatrix());
	shader->setUniform("camera_world", camera.getPosition());

	shader->setUniform("dirL.color", dirL.color);
	shader->setUniform("dirL.direction", dirL.direction);
	shader->setUniform("pointL.color", pointL.color);
	shader->setUniform("pointL.position", pointL.position);
	shader->setUniform("pointL.attenuation", pointL.attenuation);
}

void setPerFrameUniformsSkybox(Shader* shader, myCamera& camera)
{
	shader->use();
	shader->setUniform("rotation", camera.getRotationMatrix());
	shader->setUniform("projection",camera.getProjectionMatrix());
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		_dragging = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		_dragging = false;
	} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		_strafing = true;
	} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		_strafing = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_zoom -= float(yoffset) * 0.5f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// F1 - Wireframe
	// F2 - Culling
	// Esc - Exit

	if (action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		std::cout << "Key pressed" << std::endl;
		_key_pressed = key;
		return;
	}

	if (action != GLFW_RELEASE) return;

	_key_pressed = GLFW_KEY_UNKNOWN;
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_F1:
			_wireframe = !_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
			break;
		case GLFW_KEY_F2:
			_culling = !_culling;
			if (_culling) glEnable(GL_CULL_FACE);
			else glDisable(GL_CULL_FACE);
			break;
	}
}

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch (source) {
		case GL_DEBUG_SOURCE_API: {
			sourceString = "API";
			break;
		}
		case GL_DEBUG_SOURCE_APPLICATION: {
			sourceString = "Application";
			break;
		}
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
			sourceString = "Window System";
			break;
		}
		case GL_DEBUG_SOURCE_SHADER_COMPILER: {
			sourceString = "Shader Compiler";
			break;
		}
		case GL_DEBUG_SOURCE_THIRD_PARTY: {
			sourceString = "Third Party";
			break;
		}
		case GL_DEBUG_SOURCE_OTHER: {
			sourceString = "Other";
			break;
		}
		default: {
			sourceString = "Unknown";
			break;
		}
	}

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: {
			typeString = "Error";
			break;
		}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
			typeString = "Deprecated Behavior";
			break;
		}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
			typeString = "Undefined Behavior";
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY_ARB: {
			typeString = "Portability";
			break;
		}
		case GL_DEBUG_TYPE_PERFORMANCE: {
			typeString = "Performance";
			break;
		}
		case GL_DEBUG_TYPE_OTHER: {
			typeString = "Other";
			break;
		}
		default: {
			typeString = "Unknown";
			break;
		}
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: {
			severityString = "High";
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM: {
			severityString = "Medium";
			break;
		}
		case GL_DEBUG_SEVERITY_LOW: {
			severityString = "Low";
			break;
		}
		default: {
			severityString = "Unknown";
			break;
		}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}
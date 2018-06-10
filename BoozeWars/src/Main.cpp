/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/


#include "Utils.h"

#include <sstream>

#include "Geometry.h"
#include "LineGeometry.h"
#include "myCamera.h"
#include "Shader.h"
#include "ShadowMapSimple.h"
#include "Material.h"
#include "Light.h"

#include "ModelFactory.h"

#include "Texture.h"

#include "Character.h"
#include "DrunkCity.h"

#include "Exceptions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Wave.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <PhysX/PxPhysicsAPI.h>
using namespace physx;

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void setPerFrameUniforms(Shader* shader, myCamera& camera, DirectionalLight& dirL, PointLight& pointL, glm::mat4& lightMatrix);
void setPerFrameUniformsSkybox(Shader* shader, myCamera& camera);


/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

static bool _wireframe = false;
static bool _culling = true;
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 12.0f;
unsigned char **tex;
static int width = 800;
static int height = 800;
static bool _starting = false;


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
	bool fullscreen = bool(reader.GetBoolean("window", "fullscreen", false));
	int refreshRate = reader.GetInteger("window", "refreshRate", 60);

	/* --------------------------------------------- */
	// Create context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Request OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Request core profile													  
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  // Create an OpenGL debug context 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent window resizing because viewport would have to resize as well (-> not needed in this course)
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
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
	
	//Fullscreen or windowed
	if (fullscreen) 
	{
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, window_width, window_height, refreshRate);
	}
	

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

	Character* charactorService = new Character(window);
	charactorService->setFontSize(54);

	// set up Physx
	//Foundation
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation* gFoundation = nullptr;
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	PxPvdTransport* mTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10000);
	PxPvdInstrumentationFlags mPvdFlags = PxPvdInstrumentationFlag::eALL;
	PxPvd* mPvd = PxCreatePvd(*gFoundation);
	//SDK
	PxPhysics* gPhysicsSDK = nullptr;
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),false,mPvd);
	if (gPhysicsSDK == nullptr) {
		std::cout << "Failed to create Physx SDK" << std::endl;
	}
	

	if (!mPvd->connect(*mTransport, mPvdFlags)) { std::cout << "Error connecting" << std::endl; }
	

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
		//Shaders
		std::shared_ptr<Shader> textureShader = std::make_shared<Shader>("texture.vert", "textureCell.frag");
		std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("skybox.vert", "skybox.frag");
		std::shared_ptr<Shader> infiniGreen = std::make_shared<Shader>("texture.vert", "infiniGreen.frag");
		std::shared_ptr<Shader> translucent = std::make_shared<Shader>("texture.vert", "translucentRed.frag");
		Shader shadowShader = Shader("shadowShader.vert", "shadowShader.frag");
		Shader objectShader = Shader("texture.vert", "texture.frag");
		//Textures
		std::shared_ptr<Texture> sunTexture = std::make_shared<Texture>("sun.dds");
		std::shared_ptr<Texture> moonTexture = std::make_shared<Texture>("moon.dds");
		std::shared_ptr<Texture> earthTexture = std::make_shared<Texture>("earth.dds");
		std::shared_ptr<Texture> saloonTexture = std::make_shared<Texture>("objects/saloon.mtl");
		std::shared_ptr<Texture> brickTexture = std::make_shared<Texture>("bricks_diffuse.dds");
		std::shared_ptr<Texture> woodTexture = std::make_shared<Texture>("wood_texture.dds");
		std::shared_ptr<Texture> mapTexture = std::make_shared<Texture>();
		mapTexture->loadImage("map.png");

		// instancing ModelFactory and loading Models
		ModelFactory::Instance();
	
		// Create materials
		std::shared_ptr<Material> sunMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.1f, 0.9f, 0.4f), 8.0f, sunTexture);
		std::shared_ptr<Material> earthMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.5f), 5.0f, earthTexture);
		std::shared_ptr<Material> moonMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.1f, 0.9f, 0.0f), 1.0f, woodTexture);
		std::shared_ptr<Material> brickMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.2f, 0.8f, 0.5f), 5.0f, brickTexture);
		std::shared_ptr<Material> infiniGreenMat = std::make_shared<Material>(infiniGreen, glm::vec3(0.5f, 0.5f, 0.5f), 3.0f);
		std::shared_ptr<Material> translucentRed = std::make_shared<Material>(translucent, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
		std::shared_ptr<Material> mapMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.1f, 0.9f, 0.0f), 50.0f, mapTexture);
		
		//Geometries
		//Create World
		DrunkCity world = DrunkCity(300.0f, 9000.0f, 300.0f);
		//Create Physx scene
		PxScene* gScene = world.initPhysics(gPhysicsSDK);
		//Create Ground
		Geometry ground = Geometry(glm::mat4(1.0f), Geometry::createInfinitePlane(), infiniGreenMat);
		//Create map
		Geometry map = Geometry(glm::translate(glm::mat4(1.0f),glm::vec3(150,0,150)), Geometry::createRectangle(300, 300), mapMaterial);
		PxTransform groundPos(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, -1.0f, 0.0f)));
		PxRigidStatic* groundPhysicsPlane = gPhysicsSDK->createRigidStatic(groundPos);
		PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.0f, 0.1f, 0.5f);
		groundPhysicsPlane->createShape(PxPlaneGeometry(), *mMaterial);
		gScene->addActor(*groundPhysicsPlane);
		//Helper Rectangle for building placment
		Geometry cameraPlacement = Geometry(glm::mat4(1.0f), Geometry::createRectangle(Building::getLength(), Building::getWidth()), translucentRed);
		LineGeometry cameraCircle = LineGeometry(glm::mat4(1.0f), LineGeometry::createCircle(30,Building::getRange()), translucentRed);
		//Enemy
		//std::shared_ptr<Geometry> enemyModel = std::make_shared<Geometry>(glm::mat4(1.0f), Geometry::createCubeGeometry(4, 6, 4), sunMaterial);
		std::shared_ptr<Geometry> enemyModel = std::make_shared<Geometry>(glm::scale(glm::mat4(1.0f),glm::vec3(2.0f,3.0f,2.0f)), Geometry::createSphereGeometry(20,20,1), sunMaterial);
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

		//ShadowMap
		ShadowMapSimple directionalLightShadow = ShadowMapSimple();
		directionalLightShadow.init(1024, 1024);

		//Create School building
		Geometry school = Geometry(glm::translate(glm::mat4(1.0f), glm::vec3(340, -15, 240)),
									Geometry::createCubeGeometry(45, 30, 70),
									brickMaterial);
		school.addChild(std::make_unique<Geometry>(glm::translate(glm::mat4(1.0f), glm::vec3(-5, -5, 40)), 
													Geometry::createCylinderGeometry(14, 45, 10),
													brickMaterial));

		//create Wave
		Wave wave = Wave(std::list<wavetuple>({wavetuple(10,2,1),wavetuple(10,0.5,1),wavetuple(5,0.2,1),wavetuple(10,1,0.1),wavetuple(20,0.25,1)}));
		//Wave wave = Wave(std::list<wavetuple>({ wavetuple(0,2,1)}));
		//if the left click is still pressed
		bool pressing = false;

		//Geometry worldModel = Geometry(glm::mat4(1.0f), Geometry::createCubeGeometry(x, y, z), material);
		// Initialize camera
		myCamera camera(fov, float(window_width) / float(window_height), nearZ, farZ);
		PointLight pointL(glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(100, 100, 100));
		DirectionalLight dirL(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(4.0f,4.0f,4.0f));
		
		//Matrices for non moving lights for shadow map
		glm::mat4 dirLMatrix = glm::ortho(-300.0f, 300.0f, -200.0f, 200.0f,-300.0f,300.0f);
		
		glm::mat4 dirLViewM = glm::lookAt(glm::vec3(150.0f,0.0f,150.0f), glm::vec3(150.0f, 0.0f, 150.0f)+dirL.direction, glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 dirLProjView = dirLMatrix * dirLViewM;

		//set Uniforms for shadow Map


		//Test or indicator for start position
		//world.addEnemy(ModelFactory::DEFAULT_BUILDING);


		// Render loop
		double mouse_x, mouse_y;
		float t = float(glfwGetTime());
		float dt = 0.0f;
		float t_sum = 0.0f;
		float t_framecounter = 0.0f;
		float physxTime = 0.0f;
		const float physxTimestep = 1.0f / 60.0f;
		int buildingColCounter = 0;
		int streetColCounter = 0;
		bool start = false;
		bool end = false;
		bool won = false;
		int buildingLimit = 0;
		int placeMinCounter = 0;
		int frameCounter = 0;

		while   (!glfwWindowShouldClose(window))
		{
			// Update camera
			{
				glfwGetCursorPos(window, &mouse_x, &mouse_y);
				//camera.updatePosition(_key_pressed,dt);
				//front
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_W, dt);
				}
				//back
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_S, dt);
				}
				//right
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_D, dt);
				}
				//left
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_A, dt);
				}
				//up
				if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_R, dt);
				}
				//down
				if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
					camera.updatePosition(GLFW_KEY_F, dt);
				}
				camera.updateDirection(int(mouse_x), int(mouse_y));
			}
			// Set per-frame uniforms
			setPerFrameUniforms(textureShader.get(), camera, dirL, pointL,dirLProjView);
			setPerFrameUniforms(infiniGreen.get(), camera, dirL, pointL,dirLProjView);
			setPerFrameUniforms(translucent.get(), camera, dirL, pointL,dirLProjView);
			setPerFrameUniformsSkybox(skyboxShader.get(), camera);

			//GameLogic
			// Play logic
			if (start && !end) {
				try {
					if (physxTime >= physxTimestep)
					{
						physxTime -= physxTimestep;
						world.calculateForces();
						gScene->simulate(physxTimestep);
						gScene->fetchResults(true);
						world.walk();
					}
				}
				catch (int e) {
					if (e == GAME_END) {
						end = true;
					}
				}
				if (wave.spawnEnemy(dt))
				{
					world.addEnemy(ModelFactory::DEFAULT_ENEMY);
				}
				try {
					world.fight(dt);
				}
				catch (int e) {
					if (e == ALL_ENEMIES_DESTROYED && wave.waveIsFinished()) {
						won = true;
						end = true;
						
					}
				}
				physxTime += dt;
			}

			// Render
			//Shadowmap pass

		


			glViewport(0, 0, 1024, 1024);
			directionalLightShadow.bindForWriting();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shadowShader.use();
			shadowShader.setUniform("viewProjMatrix", dirLProjView);
			glCullFace(GL_FRONT);
			world.zeichne(&shadowShader);
			school.drawShadow(shadowShader);
			
			glCullFace(GL_BACK);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			

			//Render scene
			glViewport(0, 0, window_width, window_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			directionalLightShadow.bindForReading();
			ground.draw(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.1f,0.0f)));
			world.zeichne(textureShader.get()); //needs shadow

			//mapTexture->loadImage("map.png");
			map.draw();
			if (camera.getGroundIntersection() != glm::vec3(0, 1, 0)) {
				cameraPlacement.draw(glm::translate(glm::mat4(1.0f), camera.getGroundIntersection()+glm::vec3(0.0f,-0.01f,0.0f)));
				cameraCircle.draw(glm::translate(glm::mat4(1.0f), camera.getGroundIntersection() + glm::vec3(0.0f, -0.01f, 0.0f)));
			}
			school.draw(); 
			worldModel.draw();
			
			// Placement logic
			if (_dragging && !pressing && !end) {
				pressing = true;
				try {
					// trying to place building
					world.placeBuilding(camera.getGroundIntersection()[0], camera.getGroundIntersection()[2]);
				}
				catch (int e) {
					if (e == BUILDING_COLLISION) {
						buildingColCounter = 300;
					}
					if (e == STREET_COLLISION) {
						streetColCounter = 300;
					}
					if (e == BUILDINGS_LIMIT_REACHED) {
						buildingLimit = 200;
					}
				}
			}

			if (_starting) {
				if (!world.hasMinOneBuildings()) {
					_starting = false;
					placeMinCounter = 200;
				}
				else {
					start = true;
				}
			}

			if (placeMinCounter > 0) {
				charactorService->renderText("PLACE MININUM 1 BUILDING BEFORE STARTING!", 0, 600, 0.5f, glm::vec3(1.0, 0.0, 0.0));
				placeMinCounter--;
			}

			if (buildingColCounter > 0) {
				charactorService->renderText("COLLIDING WITH BUILDING!", 20, window_height - 250, 0.8f, glm::vec3(1.0, 0.0, 0.0));
				buildingColCounter--;
			}

			if (streetColCounter > 0) {
				charactorService->renderText("COLLIDING WITH STREET!", 20, window_height - 200, 0.8f, glm::vec3(1.0, 0.0, 0.0));
				streetColCounter--;
			}

			if (buildingLimit > 0) {
				charactorService->renderText("BUILDING LIMIT REACHED!", 20, window_height - 100, 0.8f, glm::vec3(1.0, 0.0, 0.0));
				buildingLimit--;
			}

			if (end) 
			{
				if (won) 
				{
					charactorService->renderText("YOU WIN", 20, 600, 3.0f, glm::vec3(1.0, 0.0, 0.0));
				}
				else 
				{
					charactorService->renderText("YOU LOSE!", 20, 600, 2.5f, glm::vec3(1.0, 0.0, 0.0));
				}
			}

			if (!_dragging && pressing) {
				pressing = false;
			}



			// player informations
			std::string life = "LIFE: ";
			std::ostringstream lifeAmout;
			lifeAmout << world.getHP();
			life += lifeAmout.str();
			glm::vec3 color;
			if (world.getHP() <= 20) {
				color = glm::vec3(1.0, 0.0, 0.0);
			}
			else {
				color = glm::vec3(0.0, 0.0, 1.0);
			}
			charactorService->renderText(life.c_str(), 100, 100, 0.8f, color);

			std::string buildings = "BUILDINGS: ";
			std::ostringstream left;
			left << world.getBuildingsLeft();
			buildings += left.str();
			if (world.getBuildingsLeft() <= 0) {
				color = glm::vec3(1.0, 0.0, 0.0);
			}
			else {
				color = glm::vec3(0.0, 0.0, 1.0);
			}
			charactorService->renderText(buildings.c_str(), 450, 100, 0.8f, color);

			if (!start && world.hasMinOneBuildings()) {
				charactorService->renderText("PRESS ENTER TO START WAVE", 20, window_height-50, 0.5f, glm::vec3(0.0,0.5,0.5));
			}

			// Compute frame time
			dt = t;
			t = float(glfwGetTime());
			dt = t - dt;
			t_framecounter += dt;
			t_sum += dt;
			frameCounter++;

			 
			//FPS (0.5sec avarage)
			if (t_framecounter > 1.0f) 
			{
				std::cout << frameCounter / t_framecounter << std::endl;
				frameCounter = 0;
				t_framecounter -= 1.0f;
			}
			
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
	// Destroy PhysX
	/* --------------------------------------------- */
	
	gPhysicsSDK->release();
	if (gFoundation != nullptr) {
		gFoundation->release();
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


void setPerFrameUniforms(Shader* shader, myCamera& camera, DirectionalLight& dirL, PointLight& pointL,glm::mat4& lightMatrix)
{
	shader->use();
	shader->setUniform("viewProjMatrix", camera.getViewProjectionMatrix());
	shader->setUniform("camera_world", camera.getPosition());
	shader->setUniform("lightMatrix", lightMatrix);

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

	if (action != GLFW_RELEASE) return;

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
		case GLFW_KEY_ENTER:
			_starting = true;
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
#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"

#include "Mesh.h"

#include "Shader.h"

#include "myCamera.h"

class Model
{
private:
protected:
	//variables
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	GLenum image_format = GL_RGB;		//format the image is in
	GLint internal_format = GL_RGB;		//format to store the image in
	GLint level = 0;					//mipmapping level
	GLint border = 0;					//border size
										//methods
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model();
	Model(std::string path);
	void draw(std::shared_ptr<Shader> shader);
	GLint loadTexture(const char* filename);

	//void Move(GLfloat x, GLfloat y);
	~Model();
};
#endif

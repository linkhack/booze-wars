/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <string>
#include <GL/glew.h>
#include "Utils.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "FreeImage.h"
#include <iostream>



/* --------------------------------------------- */
// 2D texture
/* --------------------------------------------- */

class Texture
{
private:
	GLuint _handle;
	bool _init;

public:
	GLuint id;
	std::string type;
	aiString path;
	Texture();
	Texture(std::string file);
	~Texture();

	void bind(unsigned int unit);
	void loadImage(std::string fileName);
};

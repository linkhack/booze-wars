#version 330 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
	vec4 shadowCoords;
} vert;


uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;
uniform mat3 normalMatrix;
uniform mat4 lightMatrix;	


void main() {
	vert.normal_world = normalMatrix * normal;
	vert.uv = uv;
	vec4 position_world_ = modelMatrix * position;
	vert.position_world = position_world_.xyz;
	vert.shadowCoords = lightMatrix * position_world_;
	gl_Position = viewProjMatrix * position_world_;
}
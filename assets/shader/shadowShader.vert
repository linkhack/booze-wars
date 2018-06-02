#version 330 core
layout (location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;

void main()
{
    vec4 pos = viewProjMatrix * modelMatrix * position;
    gl_Position = pos;
}  		
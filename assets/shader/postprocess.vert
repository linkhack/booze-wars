#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out textureCoords;

void main()
{
    glPosition = vec4(position,1);
	textureCoords = uv;
}  		
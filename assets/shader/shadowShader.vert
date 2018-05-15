#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;

void main()
{
    vec4 pos = projection * rotation * vec4(position, 1.0);
    gl_Position = pos;
}  
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec4 ParticleColor;

uniform mat4 viewProjMatrix;
uniform vec3 position;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    ParticleColor = color;
    gl_Position = viewProjMatrix * vec4((vertex.xyz * scale) + position, 1.0);
}
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec4 ParticleColor;

uniform mat4 viewProjMatrix;
uniform vec3 position;
uniform vec4 color;

void main()
{
    float scale = 0.2f;
    ParticleColor = color;
	vec3 particelPos = vec3(vertex.x,vertex.y,0)*scale + position;
    gl_Position = viewProjMatrix * vec4(particelPos, 1.0);
}
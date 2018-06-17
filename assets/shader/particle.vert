#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec3 position; // <vec2 position, vec2 texCoords>
layout (location = 2) in vec4 color; // <vec2 position, vec2 texCoords>

out vec4 ParticleColor;

uniform mat4 viewProjMatrix;

void main()
{
    float scale = 0.1f;
	
    ParticleColor = color;
	vec3 particelPos = vec3(vertex.x,vertex.y,0)*scale + position;
    gl_Position = viewProjMatrix * vec4(particelPos, 1.0);
}
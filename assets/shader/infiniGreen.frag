#version 330 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
	vec4 shadowCoords;
} vert;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;

uniform vec3 camera_world;

uniform vec3 materialCoefficients; // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha;
uniform sampler2D diffuseTexture;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;

uniform struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
} pointL;
float fogFunction(float distance, float start, float end)
{
	return clamp((distance-start)/(end-start),0,1);
}

vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
	float d = length(l);
	l = normalize(l);
	float att = 1.0;	
	if(attenuate) att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
	vec3 r = reflect(-l, n);
	return (diffuseF * diffuseC * max(0, dot(n, l)) + specularF * specularC * pow(max(0, dot(r, v)), alpha)) * att; 
}

void main() {	
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);
	vec4 fogColor = vec4(0.45,0.45,0.6,1);
	vec3 texColor = 0.35*vec3(0.34313,0.429411,0.109803)+0.25*vec3(0.58823529411,0.42745098039,0.1333333);;
	color = vec4(texColor * materialCoefficients.x, 1); // ambient
	
	// add directional light contribution
	color.rgb += phong(n, -dirL.direction, v, dirL.color * texColor, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));
			
	// add point light contribution
	color.rgb += phong(n, pointL.position - vert.position_world, v, pointL.color * texColor, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
	
	float fogDistance = gl_FragCoord.z/gl_FragCoord.w;
	float fogAmount = fogFunction(fogDistance,100,450);
	

	color = mix(color, fogColor,fogAmount);
	normal =  0.5*vec4(n,1)+0.5;
}


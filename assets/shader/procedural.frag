#version 330 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

uniform sampler2D diffuseTexture;
uniform sampler2DShadow shadowMap;
//uniform sampler2D specularTexture;

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


uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;

uniform struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
} pointL;

vec3 poissonDisk[16] = vec3[]( 
   vec3( -0.94201624, -0.39906216 , 0 ), 
   vec3( 0.94558609, -0.76890725 , 0 ), 
   vec3( -0.094184101, -0.92938870 , 0 ), 
   vec3( 0.34495938, 0.29387760 , 0 ), 
   vec3( -0.91588581, 0.45771432 , 0 ), 
   vec3( -0.81544232, -0.87912464 , 0 ), 
   vec3( -0.38277543, 0.27676845 , 0 ), 
   vec3( 0.97484398, 0.75648379 , 0 ), 
   vec3( 0.44323325, -0.97511554 , 0 ), 
   vec3( 0.53742981, -0.47373420 , 0 ), 
   vec3( -0.26496911, -0.41893023 , 0 ), 
   vec3( 0.79197514, 0.19090188 , 0 ), 
   vec3( -0.24188840, 0.99706507 , 0 ), 
   vec3( -0.81409955, 0.91437590 , 0 ), 
   vec3( 0.19984126, 0.78641367 , 0 ), 
   vec3( 0.14383161, -0.14100790 , 0 ) 
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float random2d(vec2 x){
	return random(vec3(sin(x.x),cos(x.y),0),0);
}

//based on https://thebookofshaders.com/13/
float noise(vec2 coord){
	vec2 integer = floor(coord);
	vec2 fractional = fract(coord);
	//four corners
	float a = random2d(integer);
	float b = random2d(integer+vec2(0.6,0.0));
	float c = random2d(integer+vec2(0.0,0.6));
	float d = random2d(integer+vec2(1.0,1.0));
	
	//mixing factor:
	vec2 u = fractional*fractional*(3.0-2.0*fractional);
	return mix(a,b,u.x)+(c-a)*u.y*(1.0-u.x)+(d-b)*u.x*u.y;
}

float fbm (vec2 coords) {
    // Initial values
    float value = 0.0;
    float amplitude = 2;
    float frequency = 0.0;
    //
    // Loop of octaves
    for (int i = 0; i < 3; i++) {
		float n = noise(coords);
        value += amplitude *n*n;
        coords *= 5;
        amplitude *= .3;
    }
    return value;
}


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
	float factor = fbm(vert.uv);
	float xFactor = fbm(vert.position_world.xz);
	float yFactor = fbm(vert.position_world.zy);
	
	factor = min(floor(3*factor)/2,1);

	vec3 dirtColor = 0.8*vec3(0.1098,0.30196,0.028431);
	vec3 grassColor = 0.8*vec3(0.34313,0.429411,0.109803);
	vec3 texColor = mix(grassColor,dirtColor,factor*xFactor*yFactor);
		

	//color = vec4(texColor * materialCoefficients.x,1); // ambient
	vec4 fogColor = vec4(0.45,0.45,0.6,1);
	
	//Shadow Calculations

	float shadowDir = 0.0f;
	vec3 shadowUv = 0.5*(vert.shadowCoords.xyz)/vert.shadowCoords.w + 0.5;	
	for(int i=0;i<16;i++)
	{
		//int index = int(16.0*random(floor(vert.position_world*1000.0), i))%16;
		shadowDir += texture(shadowMap,shadowUv+poissonDisk[i]/800.0f)/16; //non random no noise but jaggies
		//shadowDir += texture(shadowMap,shadowUv+poissonDisk[index]/1000.0f)/6; //random sampling noise but smoother
	}

	// add directional light contribution
	color.rgb += shadowDir*phong(n, -dirL.direction, v, dirL.color * texColor, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));
			
	// add point light contribution
	//color.rgb += shadowDir*phong(n, pointL.position - vert.position_world, v, pointL.color * texColor, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
	
	float fogDistance = gl_FragCoord.z/gl_FragCoord.w;
	float fogAmount = fogFunction(fogDistance,100,450);
	

	//color = mix(color, fogColor,fogAmount);
	normal =  0.5*vec4(n,1)+0.5;
	color = vec4(texColor,1);
	//color = vec4(vec3(xFactor*yFactor),1);
	//color=vec4(n,1);
	//color = vec4(vec3(0.5*floor(2*pow(max(0, dot(r, v)), alpha))),1);
	//color = vec4(0,0,0,1);
	//color=vec4(n,1);
	//color = vec4(vec3(0.5*floor(2*pow(max(0, dot(r, v)), alpha))),1);
}


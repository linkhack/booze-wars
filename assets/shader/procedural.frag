#version 330 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#define PI 3.1415926535897932384626433832795
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
uniform int width;

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

// Permutation polynomial: (34x^2 + x) mod 289
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

// Value Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/lsf3WH
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
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
	float factor = 0.5-noise(vert.position_world.xz*0.05);
	//float factor2 = 0.5-noise(vert.position_world.xz*0.5);
	factor = 1-factor*factor;
	//factor = min(floor(5*factor)/5,1);
	
	float isStreet = texture(diffuseTexture,vert.position_world.zx/300).r;
	
	factor = min(floor(5*factor)/4,1);
	//factor = min(floor(5*(factor+0.1*factor2))/4,1);

	vec3 dirtColor = vec3(0.58823529411,0.42745098039,0.1333333);
	vec3 grassColor = vec3(0.34313,0.429411,0.109803);
	vec3 grassColor2 = 0.8*vec3(0.3433,0.129411,0.0509803);
	vec3 texColor = mix(dirtColor,grassColor,(1-isStreet)*factor-isStreet*factor);
		

	color = vec4(texColor * materialCoefficients.x,1); // ambient
	vec4 fogColor = vec4(0.45,0.45,0.6,1);
	
	//Shadow Calculations

	float shadowDir = 0.0f;
	vec3 shadowUv = 0.5*(vert.shadowCoords.xyz)/vert.shadowCoords.w + 0.5;	
	for(int i=0;i<4;i++)
	{
		//int index = int(16.0*random(floor(vert.position_world*1000.0), i))%16;
		shadowDir += texture(shadowMap,shadowUv+poissonDisk[i]/800.0f)/16; //non random no noise but jaggies
		//shadowDir += texture(shadowMap,shadowUv+poissonDisk[index]/1000.0f)/6; //random sampling noise but smoother
	}

	// add directional light contribution
	color.rgb += shadowDir*phong(n, -dirL.direction, v, dirL.color * texColor, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));
			
	// add point light contribution
	color.rgb += shadowDir*phong(n, pointL.position - vert.position_world, v, pointL.color * texColor, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
	
	float fogDistance = gl_FragCoord.z/gl_FragCoord.w;
	float fogAmount = fogFunction(fogDistance,100,450);
	

	color = mix(color, fogColor,fogAmount);
	normal =  0.5*vec4(n,1)+0.5;
	//color = vec4(texColor,1);
	//color = vec4(vec3(0.5-abs(noise(vert.position_world.xz*0.005))),1);
	//color=vec4(n,1);
	//color = vec4(vec3(0.5*floor(2*pow(max(0, dot(r, v)), alpha))),1);
	//color = vec4(0,0,0,1);
	//color=vec4(n,1);
	//color = vec4(vec3(0.5*floor(2*pow(max(0, dot(r, v)), alpha))),1);
}


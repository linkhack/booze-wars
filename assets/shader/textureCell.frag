#version 330 core

uniform sampler2D diffuseTexture;
uniform sampler2DShadow shadowMap;

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
	float cosAngleNormal = max(0,dot(n,l));
	float specularToon = (pow(max(0, dot(r, v)), alpha)>=0.8)?1:0;

	return (diffuseF * diffuseC * min(floor(3*cosAngleNormal)/2,1) + specularF * specularC * specularToon) * att; 
}

void main() {	
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);
	
	vec3 texColor = texture(diffuseTexture, vert.uv).rgb;
	color = vec4(texColor * materialCoefficients.x, 1); // ambient
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
	color.rgb += shadowDir*phong(n, pointL.position - vert.position_world, v, pointL.color * texColor, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
	
	//float fogDistance = gl_FragCoord.z/gl_FragCoord.w;
	//float fogAmount = fogFunction(fogDistance,100,450);
	
	//color = mix(color, fogColor,fogAmount);
	normal =  0.5*vec4(n,1)+0.5;
	//color=vec4(n,1);
	//color = vec4(vec3(0.5*floor(2*pow(max(0, dot(r, v)), alpha))),1);
}


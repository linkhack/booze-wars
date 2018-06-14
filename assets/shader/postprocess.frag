#version 330 core
out vec4 color;

in vec2 textureCoords;

uniform sampler2D colorInformation;
uniform sampler2D normalInformation;
uniform sampler2D depthInformation;



uniform float nearZ;
uniform float farZ;
uniform float width;
uniform float height;
uniform float treshold;

float normalizeZ(float x)
{
	float z = 2.0*x-1.0;
	float linearDepth = 2.0*nearZ*farZ/(farZ+nearZ - z*(farZ-nearZ));
	//return linearDepth;
	return (nearZ-linearDepth)/(farZ-nearZ);
	
}

void main()
{    
    vec2 dx = vec2(0.5*1/width,0);
	vec2 dy = vec2(0,0.5*1/height);
	vec2 p=textureCoords-dy;
	float x1 = texture(depthInformation,p-dx).x;
	vec3 y1 = texture(normalInformation,p-dx).xyz;
	x1 = normalizeZ(x1);
	float x2 = texture(depthInformation,p).x;
	vec3 y2 = texture(normalInformation,p).xyz;
	x2 = normalizeZ(x2);
	float x3 = texture(depthInformation,p+dx).x;	
	vec3 y3 = texture(normalInformation,p+dx).xyz;	
	x3 = normalizeZ(x3);
	p=textureCoords;
	float x4 = texture(depthInformation,p-dx).x;
	vec3 y4 = texture(normalInformation,p-dx).xyz;
	x4 = normalizeZ(x4);
	float x5 = texture(depthInformation,p).x;
	vec3 y5 = texture(normalInformation,p).xyz;
	x5 = normalizeZ(x5);
	float x6 = texture(depthInformation,p+dx).x;	
	vec3 y6 = texture(normalInformation,p+dx).xyz;	
	x6 = normalizeZ(x6);
	p=textureCoords+dy;
	float x7 = texture(depthInformation,p-dx).x;
	vec3 y7 = texture(normalInformation,p-dx).xyz;
	x7 = normalizeZ(x7);
	float x8 = texture(depthInformation,p).x;
	vec3 y8 = texture(normalInformation,p).xyz;
	x8 = normalizeZ(x8);
	float x9 = texture(depthInformation,p+dx).x;
	vec3 y9 = texture(normalInformation,p+dx).xyz;
	x9 = normalizeZ(x9);
	
	float Gx = x1-x3 + 2.0*(x4-x6)+ x7-x9;
	vec3 Nx = y1-y3 + 2.0*(y4-y6)+ y7-y9;
	float Gy = x1-x7 + 2.0*(x2-x8)+ x3-x6;
	vec3 Ny = y1-y7 + 2.0*(y2-y8)+ y3-y6;
	vec3 NL = y2+y4+y6+y8-4*y6;
	
	float gradient = Gx * Gx + Gy * Gy;
	//vec3 nGrad = Nx*Nx + Ny*Ny;
	//float maxNormalGrad = max(max(NL.x,NL.y),NL.z);
	float maxNormalGrad = length(NL);
	color = texture(colorInformation,textureCoords);
	float result = (gradient>treshold*treshold || maxNormalGrad>treshold)? 0:1;

	color = result * color;
	//color = vec4(y6,1);
	color.w = 1;
}
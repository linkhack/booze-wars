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
    vec2 dx = vec2(1/width,0);
	vec2 dy = vec2(0,1/height);
	vec2 p=textureCoords-dy;
	float x1 = texture(depthInformation,p-dx).x;
	x1 = normalizeZ(x1);
	float x2 = texture(depthInformation,p).x;
	x2 = normalizeZ(x2);
	float x3 = texture(depthInformation,p+dx).x;	
	x3 = normalizeZ(x3);
	p=textureCoords;
	float x4 = texture(depthInformation,p-dx).x;
	x4 = normalizeZ(x4);
	float x5 = texture(depthInformation,p).x;
	x5 = normalizeZ(x5);
	float x6 = texture(depthInformation,p+dx).x;	
	x6 = normalizeZ(x6);
	p=textureCoords+dy;
	float x7 = texture(depthInformation,p-dx).x;
	x7 = normalizeZ(x7);
	float x8 = texture(depthInformation,p).x;
	x8 = normalizeZ(x8);
	float x9 = texture(depthInformation,p+dx).x;
	x9 = normalizeZ(x9);
	
	float Gx = x1-x3 + 2.0*(x4-x6)+ x7-x9;
	float Gy = x1-x7 + 2.0*(x2-x8)+ x3-x6;
	float S = x2 + x4 + x6 + x8 - 4 * x9;
	float gradient = Gx * Gx + Gy * Gy;
	
	color = texture(colorInformation,textureCoords);
	float result = gradient>treshold? 0:1;
	color = result * color;
	color = vec4(color.xyz,1);

}
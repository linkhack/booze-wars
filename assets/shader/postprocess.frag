#version 330 core
layout(location = 0) out color;

in vec2 textureCoords;

uniform sampler2D colorInformation;
uniform sampler2D normalInformation;
uniform sampler2D depthInformation

void main()
{    
    color = texture(colorInformation,textureCoords);
}


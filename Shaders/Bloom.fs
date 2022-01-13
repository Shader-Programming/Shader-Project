#version 410 core

out vec4 FragColor;

uniform sampler2D image;
uniform sampler2D bloomblur;

in vec2 uv;

void main(){
	vec3 hdrcolor = texture(image,uv).rgb;
	vec3 bloomcolor = texture(bloomblur,uv).rgb;
	hdrcolor += bloomcolor;
	vec3 reinhard = hdrcolor/(hdrcolor+vec3(1.0));
	FragColor = vec4(hdrcolor,1.0);
}
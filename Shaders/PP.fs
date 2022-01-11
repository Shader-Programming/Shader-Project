#version 410 core

out vec4 FragColor;

uniform sampler2D image;

in vec2 uv;

void main(){
	float result = texture(image,uv).r;
	//result = result/(result+vec3(1.0f));
	FragColor = vec4(vec3(result),1.0);
}
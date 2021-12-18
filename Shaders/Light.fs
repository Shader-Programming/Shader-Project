#version 410 core

out vec4 FragColor;

uniform vec3 objectcol;
uniform vec3 lightcol;

in vec2 uv;

void main(){
	FragColor = vec4(lightcol*objectcol,1.0);
}
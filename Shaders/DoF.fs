#version 410 core

float LinearizeDepth(float depth);

out vec4 FragColor;
uniform sampler2D image;
uniform sampler2D depthmap;

in vec2 uv;
const float near_plane = 1;
const float far_plane = 100;

void main(){
	float depth = texture(depthmap,uv).b;
	float focuspoint = texture(depthmap,vec2(0.5,0.5)).b;
	float result1 = LinearizeDepth(depth)/far_plane;
	float result2 = LinearizeDepth(focuspoint)/far_plane;

	float distance = (focuspoint-depth);

	//FragColor = vec4(vec3(distance),1.0);
	if(distance < 0.1f){
		FragColor = vec4(texture(image,uv).rgb,1.0);
	}else{
		FragColor = vec4(vec3(0.0),1.0);
	}
}

float LinearizeDepth(float depth){
	float z = depth*2.0 - 1.0;
	return(2.0*near_plane*far_plane)/(far_plane+near_plane-z*(far_plane-near_plane));
}
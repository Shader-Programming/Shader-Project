
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aUV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal ;
out vec3 posWS;
out vec2 UV;


void main()
{  
    gl_Position = projection * view * model*vec4(aPos,1.0);
    normal = aNormals ;
    posWS = (model*vec4(aPos,1.0)).xyz;
    UV = aUV;
}
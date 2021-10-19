#version 410 core

out vec4 FragColor;

in vec3 normal ;
uniform vec3 lightcol;
uniform vec3 objectcol;
float ambientfactor = 0.5;

void main()
{   
    vec3 ambientcol = lightcol*objectcol*ambientfactor;
    vec3 result = ambientcol;
    FragColor = vec4(result, 1.0);
}


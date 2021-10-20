#version 410 core

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;
uniform vec3 lightcol;
uniform vec3 objectcol;
uniform vec3 lightdir;
uniform vec3 viewpos;

float ambientfactor = 0.3;
float shine = 256;
float specularstrength = 0.2;

void main()
{   
    //ambient
    vec3 norm = normalize(normal);
    vec3 ambientcol = lightcol*objectcol*ambientfactor;

    //diffuse
    float diffusefactor = dot(norm,-lightdir);
    diffusefactor = max(diffusefactor,0);
    vec3 diffusecolor = lightcol*objectcol*diffusefactor;

    //specular
    vec3 viewdir = normalize(viewpos-posWS);
    vec3 reflectdir = reflect(lightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = lightcol*specularfactor*specularstrength;

    vec3 result = ambientcol+diffusecolor+specularcol;
    FragColor = vec4(result, 1.0);
}


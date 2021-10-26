#version 410 core

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;
uniform vec3 lightcol;
uniform vec3 objectcol;
uniform vec3 lightdir;
uniform vec3 viewpos;
vec3 getdirlight(vec3 norm, vec3 viewdir);

float ambientfactor = 0.8; //0.3
float shine = 256; //256
float specularstrength = 0.2; //0.2

struct pointlight{
    vec3 pos;
    vec3 col;
    float kc;
    float kl;
    float ke;
};

uniform pointlight plight;

void main()
{   
    //ambient
    vec3 norm = normalize(normal);
    vec3 viewdir = normalize(viewpos-posWS);
    vec3 result = vec3(0,0,0);
    //result = getdirlight(norm,viewdir);

    //point light stuff
    float dist = length(plight.pos-posWS);
    float attn = 1.0/(plight.kc + (plight.kl*dist)+(plight).ke*(dist*dist));
    vec3 plightdir = normalize(plight.pos-posWS);

    vec3 ambientcol = lightcol*objectcol*ambientfactor;
    ambientcol = ambientcol*attn;

    float diffusefactor = dot(norm,plightdir);
    diffusefactor = max(diffusefactor,0.0);
    vec3 diffusecolor = lightcol*objectcol*diffusefactor;
    diffusecolor = diffusecolor*attn;

    vec3 reflectdir = reflect(plightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = lightcol*specularfactor*specularstrength;
    specularcol = specularcol*attn;
    vec3 pointlightresult = ambientcol+diffusecolor+specularcol;
    result = result+pointlightresult;
    FragColor = vec4(result, 1.0);
}

vec3 getdirlight(vec3 norm, vec3 viewdir){
    vec3 ambientcol = lightcol*objectcol*ambientfactor;

    //diffuse
    float diffusefactor = dot(norm,-lightdir);
    diffusefactor = max(diffusefactor,0);
    vec3 diffusecolor = lightcol*objectcol*diffusefactor;

    //specular
    vec3 reflectdir = reflect(lightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = lightcol*specularfactor*specularstrength;

    vec3 result = ambientcol+diffusecolor+specularcol;
    return result;
}


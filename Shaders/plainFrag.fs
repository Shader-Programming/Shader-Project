#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 normal ;
in vec3 posWS;
in vec2 UV;
in mat3 TBN;

//Shadow Uniforms
uniform sampler2D depthmap;
uniform mat4 lightspacematrix;

uniform vec3 lightcol;
uniform vec3 objectcol;
uniform vec3 lightdir;
uniform vec3 viewpos;
uniform float bloombrightness;

uniform sampler2D diffusetexture;
uniform sampler2D normalmap;
uniform sampler2D speculartexture;

float ambientfactor = 0.3; //0.3
float shine = 256; //256
float specularstrength = 0.5; //0.2

struct pointlight{
    vec3 pos;
    vec3 col;
    float kc;
    float kl;
    float ke;
};
#define NR_POINT_LIGHTS 2
uniform pointlight plights[NR_POINT_LIGHTS];

struct spotlight{
    vec3 pos;
    vec3 col;
    float kc;
    float kl;
    float ke;


    vec3 direction;

    float innerrad;
    float outerrad;
};

struct rimlight{
    vec3 pos;
    vec3 col;
    float kc;
    float kl;
    float ke;
};

vec3 getdirlight(vec3 norm, vec3 viewdir, float shadow);
vec3 getpointlight(pointlight plight, vec3 norm, vec3 viewdir);
vec3 getspotlight(vec3 norm, vec3 viewdir);
vec3 getrimlight(vec3 norm, vec3 viewdir);
float CalcShadow(vec4 fragposlightspace);
uniform spotlight slight;

void main()
{   
    vec4 posLS = lightspacematrix*vec4(posWS,1.0);
    float shadow = CalcShadow(posLS);
    vec3 newnorm = texture(normalmap,UV).xyz;
    newnorm = newnorm*2.0-1.0;
    newnorm = normalize(TBN*newnorm);

    vec3 viewdir = normalize(viewpos-posWS);
    vec3 result = vec3(0,0,0);
    result = getdirlight(newnorm,viewdir,shadow);

    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        vec3 plresult = getpointlight(plights[i],newnorm,viewdir);
        result = result + plresult;
    }

    vec3 slresult = getspotlight(newnorm,viewdir);
    result = result + slresult;
    FragColor = vec4(result, 1.0);
    float brightness = max(max(result.x,result.y),result.z);
    if(brightness > bloombrightness){
        BrightColor = FragColor;
    }else{
        BrightColor = vec4(vec3(0.0),1.0);
    }
}

vec3 getdirlight(vec3 norm, vec3 viewdir,float shadow){
    vec3 diffmapcol = texture(diffusetexture,UV).xyz;
    vec3 ambientcol = lightcol*diffmapcol*ambientfactor;

    //diffuse
    float diffusefactor = dot(norm,-lightdir);
    diffusefactor = max(diffusefactor,0);
    vec3 diffusecolor = lightcol*diffmapcol*diffusefactor;

    //specular
    vec3 halfway = normalize(lightdir+viewdir);
    float spec = pow(max(dot(norm,halfway),0.0),shine);
    vec3 specularcol = lightcol*spec*vec3(texture(speculartexture,UV).r);

    vec3 result = ambientcol+(1.0-shadow)*(diffusecolor+specularcol);
    return result;
}

vec3 getpointlight(pointlight plight, vec3 norm,vec3 viewdir){
    //point light stuff
    float dist = length(plight.pos-posWS);
    float attn = 1.0/(plight.kc + (plight.kl*dist)+(plight).ke*(dist*dist));
    vec3 plightdir = normalize(plight.pos-posWS);

    vec3 diffmapcol = texture(diffusetexture,UV).xyz;
    vec3 ambientcol = plight.col*diffmapcol*ambientfactor;
    ambientcol = ambientcol*attn;

    float diffusefactor = dot(norm,plightdir);
    diffusefactor = max(diffusefactor,0.0);
    vec3 diffusecolor = plight.col*diffmapcol*diffusefactor;
    diffusecolor = diffusecolor*attn;


    float specmapcol = texture(speculartexture,UV).r;
    vec3 reflectdir = reflect(plightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = plight.col*specularfactor*specmapcol;
    specularcol = specularcol*attn;
    vec3 result = ambientcol+diffusecolor+specularcol;
    return result;
}

vec3 getspotlight(vec3 norm, vec3 viewdir){
    //spot light stuff
    float dist = length(slight.pos-posWS);
    float attn = 1.0/(slight.kc + (slight.kl*dist)+(slight).ke*(dist*dist));
    vec3 slightdir = normalize(slight.pos-posWS);

    vec3 diffmapcol = texture(diffusetexture,UV).xyz;
    float diffusefactor = dot(norm,slightdir);
    diffusefactor = max(diffusefactor,0.0);
    vec3 diffusecolor = slight.col*diffmapcol*diffusefactor;
    diffusecolor = diffusecolor*attn;

    float specmapcol = texture(speculartexture,UV).r;
    vec3 reflectdir = reflect(slightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = slight.col*specularfactor*specmapcol;
    specularcol = specularcol*attn;

    float theta = dot(slightdir,normalize(-slight.direction));
    float denom = (slight.innerrad-slight.outerrad);
    float illumination = (theta-slight.outerrad)/denom;
    illumination = clamp(illumination,0.0,1.0);
    diffusecolor = diffusecolor*illumination;
    specularcol = specularcol*illumination;

    vec3 result = diffusecolor+specularcol;
    return result;
}

float CalcShadow(vec4 fragposlightspace){
    float bias = 0.015f;
    vec2 texelsize = 1.0/textureSize(depthmap,0);
    vec3 projcoords = fragposlightspace.xyz/fragposlightspace.w;
    projcoords = projcoords*0.5+0.5;
    float closestdepth = texture(depthmap,projcoords.xy).r;
    float currentdepth = projcoords.z;
    float shadow = 0.0;
    if(currentdepth-bias > closestdepth){
        shadow = 1.0;
    }
    return shadow;
}

//vec3 getrimlight(vec3 norm, vec3 viewdir){

//}


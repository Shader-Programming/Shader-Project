#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 normal ;
in vec3 posWS;
in vec2 UV;
in mat3 TBN;

uniform vec3 lightcol;
uniform vec3 objectcol;
uniform vec3 lightdir;
uniform vec3 viewpos;

uniform sampler2D diffusetexture;
uniform sampler2D normalmap;
uniform sampler2D speculartexture;
uniform sampler2D displacementmap;

uniform int IsNM;
uniform float PXscale;

vec3 getdirlight(vec3 norm, vec3 viewdir);
vec3 getpointlight(vec3 norm, vec3 viewdir);
vec3 getspotlight(vec3 norm, vec3 viewdir);
vec3 getrimlight(vec3 norm, vec3 viewdir);
vec2 ParallaxMapping(vec2 texcoords, vec3 viewdir);
vec3 DirectionalLight(vec3 normal,vec3 lightdir, vec3 viewdir, vec2 texcoords);

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

uniform pointlight plight;
uniform spotlight slight;

void main()
{   
    vec2 texcoords = UV;
    vec3 newnorm = texture(normalmap,UV).xyz;
    newnorm = newnorm*2.0-1.0;
    newnorm = normalize(TBN*newnorm);

    vec3 viewdir = normalize(viewpos-posWS);
    vec3 result = vec3(0,0,0);
    result = getdirlight(newnorm,viewdir);
    vec3 plresult = getpointlight(newnorm,viewdir);
    result = result + plresult;
    vec3 slresult = getspotlight(newnorm,viewdir);
    result = result + slresult;

    if(IsNM == 1){
        texcoords = ParallaxMapping(UV,viewdir);
    }
    result = result+DirectionalLight(newnorm,lightdir,viewdir,texcoords);
    FragColor = vec4(result, 1.0);
    float brightness = max(max(result.r,result.g),result.b);
    if(brightness > 0.35){
        BrightColor = FragColor;
    }else{
        BrightColor = vec4(vec3(0.0),1.0);
    }
}

vec3 getdirlight(vec3 norm, vec3 viewdir){
    vec3 diffmapcol = texture(diffusetexture,UV).xyz;
    vec3 ambientcol = lightcol*diffmapcol*ambientfactor;

    //diffuse
    float diffusefactor = dot(norm,-lightdir);
    diffusefactor = max(diffusefactor,0);
    vec3 diffusecolor = lightcol*diffmapcol*diffusefactor;

    //specular
    float specmapcol = texture(speculartexture,UV).r;
    vec3 reflectdir = reflect(lightdir,norm);
    float specularfactor = dot(viewdir,reflectdir);
    specularfactor = max(specularfactor,0.0);
    specularfactor = pow(specularfactor,shine);
    vec3 specularcol = lightcol*specularfactor*specularstrength*specmapcol;

    vec3 result = ambientcol+diffusecolor+specularcol;
    return result;
}

vec3 getpointlight(vec3 norm,vec3 viewdir){
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

    float theta = dot(-slightdir,normalize(slight.direction));
    float denom = (slight.innerrad-slight.outerrad);
    float illumination = (theta-slight.outerrad)/denom;
    illumination = clamp(illumination,0.0,1.0);
    diffusecolor = diffusecolor*illumination;
    specularcol = specularcol*illumination;

    vec3 result = diffusecolor+specularcol;
    return result;
}

vec2 ParallaxMapping(vec2 texcoords, vec3 viewdir){
    float height = texture(displacementmap,texcoords).r;
    return texcoords - (viewdir.xy)*(height*PXscale);
}

vec3 DirectionalLight(vec3 normal,vec3 lightdir, vec3 viewdir, vec2 texcoords){
    vec3 ambient = texture(diffusetexture,texcoords).rgb;//*light.ambient;
    float dif = max(dot(normal,lightdir),0.0);
    vec3 diffuse = dif*(texture(diffusetexture,texcoords)).rgb;//*light.diffuse;
    //vec3 reflectdir = reflect(-lightdir,normal);
    //spec = pow(max(dot(viewdir,reflectdir),0.0),mat.shine);
    vec3 halfway = normalize(lightdir+viewdir);
    float spec = pow(max(dot(normal,halfway),0.0),shine);
    vec3 specular = spec*vec3(texture(speculartexture,texcoords).r);
    return ambient + diffuse + specular;
}


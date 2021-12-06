#version 410 core

out vec4 FragColor;

in vec3 normal ;
in vec3 posWS;
in vec2 UV;
in mat3 TBN;


vec3 getdirlight(vec3 norm, vec3 viewdir);
vec3 getpointlight(vec3 norm, vec3 viewdir);
vec2 SteepParallaxMapping(vec2 texcoords, vec3 viewdir);
vec2 ParallaxMapping(vec2 texcoords, vec3 viewdir);
vec3 DirectionalLight(vec3 normal,vec3 lightdir, vec3 viewdir, vec2 texcoords);

struct defaultlight{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct material{
    vec3 ambient;
    sampler2D diffusetexture;
    sampler2D normalmap;
    sampler2D speculartexture;
    sampler2D displacementmap;
    float shine;
};

uniform defaultlight light;
uniform material mat;
uniform vec3 lightpos;
uniform float PXscale;
uniform vec3 lightcol;
uniform vec3 objectcol;
uniform vec3 lightdir;
uniform vec3 viewpos;

uniform int IsNM;

void main()
{   
    vec3 newnorm = normalize(normal);
    vec2 texcoords = UV;
    vec3 result  = vec3(0.0);
    vec3 lightdir = normalize(-lightpos);
    vec3 viewdir = normalize(viewpos-posWS);
    if(IsNM == 1){
        texcoords = ParallaxMapping(UV,viewdir);
    }

    newnorm = texture(mat.normalmap,texcoords).rgb;
    newnorm = normal*2.0 - 1.0;
    newnorm = normalize(TBN*newnorm);

    result = result+DirectionalLight(newnorm,lightdir,viewdir,texcoords);
    FragColor = vec4(result, 1.0);
}

vec3 DirectionalLight(vec3 normal,vec3 lightdir, vec3 viewdir, vec2 texcoords){
    vec3 ambient = texture(mat.diffusetexture,texcoords).rgb*mat.ambient;//*light.ambient;
    float dif = max(dot(normal,lightdir),0.0);
    vec3 diffuse = dif*(texture(mat.diffusetexture,texcoords)).rgb;//*light.diffuse;
    //vec3 reflectdir = reflect(-lightdir,normal);
    //spec = pow(max(dot(viewdir,reflectdir),0.0),mat.shine);
    vec3 halfway = normalize(lightdir+viewdir);
    float spec = pow(max(dot(normal,halfway),0.0),mat.shine);
    vec3 specular = light.specular*spec*vec3(texture(mat.speculartexture,texcoords).r);
    return ambient + diffuse + specular;
}

vec2 ParallaxMapping(vec2 texcoords, vec3 viewdir){
    float height = texture(mat.displacementmap,texcoords).r;
    return texcoords - (viewdir.xy)*(height*PXscale);
}
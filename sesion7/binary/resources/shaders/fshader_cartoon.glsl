#version 330 core

struct Light {
    vec3  position;
    vec3  direction;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float innerCutOff;
    float outerCutOff;
    float c0;
    float c1;
    float c2;
};

struct Material {
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec4  emissive;
    float shininess;
};

#define NLD 1
#define NLP 1
#define NLF 2

uniform vec3     ueye;
uniform Light    ulightG;
uniform Light    ulightD[NLD];
uniform Light    ulightP[NLP];
uniform Light    ulightF[NLF];
uniform Material umaterial;

in  vec3 vnor;
in  vec3 vpos;
out vec4 outColor;

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V);
vec3 funPositional (Light light, Material material, vec3 N, vec3 V);
vec3 funFocal      (Light light, Material material, vec3 N, vec3 V);

void main() {

    vec3 N = normalize(vnor);
    vec3 V = normalize(ueye - vpos);

    vec3 color = umaterial.emissive.rgb + ulightG.ambient * umaterial.ambient.rgb;
    for(int i=0; i<NLD; i++) color += funDirectional(ulightD[i],umaterial,N,V);
    for(int i=0; i<NLP; i++) color += funPositional (ulightP[i],umaterial,N,V);
    for(int i=0; i<NLF; i++) color += funFocal      (ulightF[i],umaterial,N,V);

    if     (color.r > 0.75) color = vec3(0.8, 0.8, 0.8);
    else if(color.r > 0.50) color = vec3(0.8, 0.8, 0.3);
    else if(color.r > 0.25) color = vec3(0.2, 0.8, 0.2);
    else                    color = vec3(0.8, 0.1, 0.8);

    outColor = vec4(color, umaterial.diffuse.a);

}

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(-light.direction);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    vec3 color = ambient + diffuse + specular;

    return color;

}

vec3 funPositional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3 color = attenuation*(ambient + diffuse + specular);

    return color;

}

vec3 funFocal(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3  direction   = normalize(-light.direction);
    float intensity   = clamp((light.outerCutOff - dot(L,direction))/(light.outerCutOff - light.innerCutOff), 0.0, 1.0);

    vec3 color = attenuation*(ambient + intensity*(diffuse + specular));

    return color;

}

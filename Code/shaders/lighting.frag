#version 330 core

in vec4 position; // raw position in the model coord
in vec4 positionLS;
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

uniform sampler2D shadowMap;

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;

void main (void){
    vec4 pos = modelview * position;
    vec3 n = normalize(inverse(transpose(mat3(modelview))) * normal);
    vec3 viewDir = -normalize(vec3(pos));
    
    fragColor = emision;
    vec4 pos_LS = view * lightpositions[0];
    vec3 dir_LS = normalize(pos.w * vec3(pos_LS) - pos_LS.w * vec3(pos));
    vec3 h = normalize(viewDir + dir_LS);
    // Bias to remove shadow acne/peter panning
    float bias = max(0.005f * (1.0f - dot(n, dir_LS)), 0.005f);
    vec3 projCoords = (positionLS.xyz / positionLS.w) * 0.5f + 0.5f;
    float currentDepth = projCoords.z;
    
    /* Pixelated Shadow
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    */
    
    // PCF Shadow
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    float shadow = 0.0f;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    shadow /= 9.0f;
 
    fragColor += (ambient + (1.0f - shadow) * (max(dot(n, dir_LS), 0.0f) * diffuse + pow(max(dot(n, h), 0.0f), shininess) * specular)) * lightcolors[0];
}
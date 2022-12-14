# version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 modelviewLS;
uniform mat4 projectionLS;

out vec4 position;
out vec4 positionLS;
out vec3 normal;

void main(){
    gl_Position = projection * modelview * vec4(vertex_position, 1.0f);
    // forward the raw position and normal in the model coord to frag shader
    position = vec4(vertex_position, 1.0f);
    positionLS = projectionLS * modelviewLS * vec4(vertex_position, 1.0f);
    normal = vertex_normal;
}

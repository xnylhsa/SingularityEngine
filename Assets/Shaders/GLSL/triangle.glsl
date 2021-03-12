
// Flat Color Shader

#type vertex
#version 450


layout (location = 0) in vec3 position;
layout (location = 1) in vec4 inColor;

struct (binding = 0) mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
}

layout (location = 0) out vec4 outFragColor;

void main() {
    gl_Position = vec4(position.xyz, 1.0);
    outFragColor = inColor;
}


#type fragment
#version 450

layout (location = 0) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main(){
	outColor = inColor;
}
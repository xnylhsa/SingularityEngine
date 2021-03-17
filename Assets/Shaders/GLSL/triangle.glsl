
// Flat Color Shader

#type vertex
#version 450


layout (location = 0) in vec3 position;
layout (location = 1) in vec4 inColor;

layout (push_constant) uniform Transformation
{
	mat4 proj;
	mat4 view;
	mat4 transform;
};

layout (location = 0) out vec4 outFragColor;

void main() {

    gl_Position = proj * view * transform * vec4(position.xyz, 1.0);
    outFragColor = inColor;
}


#type fragment
#version 450

layout (location = 0) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main(){
	outColor = inColor;
}
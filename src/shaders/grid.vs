#version 440 core

layout (location = 0) in vec3 aPos;

out vec3 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    VertexColor = vec3(0.0f);
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}

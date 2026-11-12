#version 440 core

layout (location = 0) in vec3 aPos;

out vec3 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    VertexColor = vec3(0.3, 0.8, 1.0);
}

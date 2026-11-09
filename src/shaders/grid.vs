#version 440 core

layout (location = 0) in vec3 aPos;

out vec3 VertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    VertexColor = vec3(1.0f, 1.0f, 1.0f);
}

#version 440 core

layout (location = 0) in vec3 aPos;

out vec3 VertexColor;

uniform float zoom;

void main()
{
    gl_Position = vec4(aPos, zoom);
    VertexColor = vec3(0.5, 0, 1.0);
}

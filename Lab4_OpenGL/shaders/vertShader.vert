#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 MVP;
uniform int isHelix;
uniform int totalPoints;

out vec3 vColor;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    if (isHelix == 1)
        gl_PointSize = 1.0 + 8.0 * float(gl_VertexID) / float(totalPoints);
    else
        gl_PointSize = 4.0;
    vColor = color;
}
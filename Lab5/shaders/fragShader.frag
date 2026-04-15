#version 330 core

in vec3 vColor;
out vec4 fCol;

void main()
{
    fCol = vec4(vColor, 1.0);
}
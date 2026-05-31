#pragma once
#include <GL/glew.h>
#include <vector>

struct Vtx
{
    float x, y, z, r, g, b;
};

GLuint setupGeometry(const std::vector<Vtx> &verts);
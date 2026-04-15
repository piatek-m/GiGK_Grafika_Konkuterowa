#pragma once
#include <GL/glew.h>
#include <string>

GLuint loadShader(const std::string &vertPath, const std::string &fragPath);

GLuint compileShader(GLenum type, const char *src);

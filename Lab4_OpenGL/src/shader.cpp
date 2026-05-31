#include "shader.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

GLuint compileShader(GLenum type, const char *src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    int ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char buf[512];
        glGetShaderInfoLog(s, 512, nullptr, buf);
        std::cerr << "[błąd shadera] " << buf << "\n";
    }
    return s;
}

GLuint loadShader(const std::string &vertPath, const std::string &fragPath)
{
    std::ifstream vertFile(vertPath);
    std::stringstream vertStrStream;
    vertStrStream << vertFile.rdbuf();
    std::string vertSrc = vertStrStream.str();

    std::ifstream fragFile(fragPath);
    std::stringstream fragStrStream;
    fragStrStream << fragFile.rdbuf();
    std::string fragSrc = fragStrStream.str();

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc.c_str());
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    int success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(prog, 512, nullptr, log);
        std::cout << "LINK ERROR: " << log << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}
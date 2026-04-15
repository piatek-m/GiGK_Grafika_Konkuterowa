#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "geometry.h"
#include <iostream>
#include <cmath>

float rotX = 0, rotY = 0, rotZ = 0;
int objectNumber = 1;

GLuint vaoHelix, vaoPyramid;
int helixCount, pyramidCount;

std::vector<Vtx> makeHelix(int N = 4, int stepsPerTurn = 60)
{
    std::vector<Vtx> v;
    int total = N * stepsPerTurn;
    float r = 0.5f;
    float height = 2.0f;

    for (int i = 0; i <= total; ++i)
    {
        float t = (float)i / total;
        float ang = t * N * 2.0f * 3.14159f;

        float x = r * cos(ang);
        float y = t * height - height / 2.0f;
        float z = r * sin(ang);

        float r_ = 0.5f + 0.5f * sin(ang);
        float g_ = 0.5f + 0.5f * sin(ang + 2.094f);
        float b_ = 0.5f + 0.5f * sin(ang + 4.188f);

        v.push_back({x, y, z, r_, g_, b_});
    }
    return v;
}

struct PyramidData
{
    std::vector<Vtx> base, side;
};

int pyramidBaseCount, pyramidSideCount;

GLuint vaoSide;
int N = 4;
float radius = 0.6f;

void keyCallback(GLFWwindow *w, int key, int /*sc*/, int action, int /*mods*/)
{
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    float step = 5.0f;
    switch (key)
    {
    case GLFW_KEY_1:
        objectNumber = 1;
        break;
    case GLFW_KEY_2:
        objectNumber = 2;
        break;
    case GLFW_KEY_UP:
        rotX -= step;
        break;
    case GLFW_KEY_DOWN:
        rotX += step;
        break;
    case GLFW_KEY_LEFT:
        rotY -= step;
        break;
    case GLFW_KEY_RIGHT:
        rotY += step;
        break;
    case GLFW_KEY_PAGE_UP:
        rotZ += step;
        break;
    case GLFW_KEY_PAGE_DOWN:
        rotZ -= step;
        break;
    case GLFW_KEY_HOME:
        rotX = rotY = rotZ = 0;
        break;
    }
}

int main()
{
    // Window init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(1200, 600, "Zadanie OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glfwSetKeyCallback(window, keyCallback);

    // Shaders
    GLuint program = loadShader("shaders/vertShader.vert", "shaders/fragShader.frag");

    // Geometry
    std::vector<Vtx> verts = {
        {-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f},
        {0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f},
        {0.0f, 0.5f, 0.0f, 0.f, 0.f, 1.f},
    };
    GLuint vao = setupGeometry(verts);

    // Corkscrew
    auto helixVerts = makeHelix(4, 80);
    helixCount = (int)helixVerts.size();
    vaoHelix = setupGeometry(helixVerts);

    // Pyramid
    std::vector<Vtx> sideTriangle = {
        {0.0f, 0.0f, 0.0f, 1, 0, 0}, // apex
        {1.0f, 0.0f, 0.0f, 0, 1, 0}, // base right
        {0.5f, 1.0f, 0.0f, 0, 0, 1}  // height
    };
    vaoSide = setupGeometry(sideTriangle);

    GLint uMVP = glGetUniformLocation(program, "MVP");
    GLint uIsHelix = glGetUniformLocation(program, "isHelix");
    GLint uTotalPts = glGetUniformLocation(program, "totalPoints");

    // Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.12f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(45.f), 1200.f / 600.f, 0.1f, 50.f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.f, 1.f, 4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 model(1.f);
        model = glm::rotate(model, glm::radians(rotX), {1.f, 0.f, 0.f});
        model = glm::rotate(model, glm::radians(rotY), {0.f, 1.f, 0.f});
        model = glm::rotate(model, glm::radians(rotZ), {0.f, 0.f, 1.f});
        glm::mat4 MVP = proj * view * model;

        glUseProgram(program);
        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(MVP));

        if (objectNumber == 1)
        {
            glUniform1i(uIsHelix, 1);
            glUniform1i(uTotalPts, helixCount);
            glBindVertexArray(vaoHelix);
            glDrawArrays(GL_POINTS, 0, helixCount);
        }
        else
        {
            // ===== PODSTAWA =====
            std::vector<Vtx> base;
            float baseY = -0.5f;

            base.push_back({0, baseY, 0, 0.8f, 0.6f, 0.2f});

            for (int i = 0; i <= N; i++)
            {
                float ang = i * 2.0f * M_PI / N;
                float x = radius * cos(ang);
                float z = radius * sin(ang);
                base.push_back({x, baseY, z, 0.9f, 0.7f, 0.1f});
            }

            GLuint vaoBase = setupGeometry(base);

            glBindVertexArray(vaoBase);
            glDrawArrays(GL_TRIANGLE_FAN, 0, base.size());

            glDeleteVertexArrays(1, &vaoBase);

            for (int i = 0; i < N; i++)
            {
                float ang = i * 2.0f * M_PI / N;

                glm::mat4 model = glm::mat4(1.0f);

                model = glm::translate(model, glm::vec3(0, baseY, 0)); // PRZESUNIĘCIE DO PODSTAWY
                model = glm::rotate(model, ang, glm::vec3(0, 1, 0));
                model = glm::translate(model, glm::vec3(radius, 0, 0)); // promień

                glm::mat4 MVP = proj * view * model;

                glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(MVP));

                glBindVertexArray(vaoSide);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Clleanp
    glfwTerminate();
    return 0;
}
#include "geometry.h"
#include "shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

constexpr int N = 4;

std::vector<Vtx> makeHelix(int turns = 4, int stepsPerTurn = 80)
{
  std::vector<Vtx> helixVerts;
  int total = turns * stepsPerTurn;
  float r = 0.5f, height = 2.0f;

  for (int i = total; i >= 0; --i)
  {
    float t = (float)i / total;
    float ang = t * turns * 2.0f * M_PI;
    float x = r * std::cosf(ang);
    float y = t * height - height / 2.0f;
    float z = r * std::sinf(ang);

    // Color channels
    float r_ch = 0.5f + 0.5f * std::sinf(ang);
    float g_ch = 0.5f + 0.5f * std::sinf(ang + 2 * M_PI / 3);
    float b_ch = 0.5f + 0.5f * std::sinf(ang + 4 * M_PI / 3);

    helixVerts.push_back({x, y, z, r_ch, g_ch, b_ch});
  }
  return helixVerts;
}

std::vector<Vtx> makePyramidBase(float radius, float baseY)
{
  std::vector<Vtx> base;
  base.push_back({0.f, baseY, 0.f, 0.8f, 0.6f, 0.2f}); // center
  for (int i = 0; i <= N; ++i)
  {
    float ang = i * 2.0f * M_PI / N;
    base.push_back({radius * std::cosf(ang), baseY, radius * std::sinf(ang), 0.9f, 0.7f, 0.1f});
  }
  return base;
}

std::vector<Vtx> makePyramidFace(float radius, float baseY, float apexY)
{
  float ang1 = 2.0f * M_PI / N;
  std::vector<Vtx> sideTriangle = {
      {radius, baseY, 0.0f, // 0 degrees
       1.0f, 0.5f, 0.1f},
      {radius * std::cosf(ang1), baseY, radius * std::sinf(ang1), // 90 degrees
       0.8f, 0.3f, 0.1f},
      {0.0f, apexY, 0.0f, // apex
       1.0f, 0.9f, 0.2f},
  };
  return sideTriangle;
}

// Inital values for user controlled variables
float rotX = 0, rotY = 0, rotZ = 0;
int objectNumber = 1;

void keyCallback(GLFWwindow * /*w*/, int key, int /*sc*/, int action, int /*mods*/)
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

  /* ---- Corkscrew ----- */
  const auto helixVerts = makeHelix();
  const int helixCount = (int)helixVerts.size();
  GLuint vaoHelix = setupGeometry(helixVerts);

  /* ---- Pyramid ------ */
  constexpr float radius = 1.0f;
  constexpr float baseY = 0.01f;
  constexpr float apexY = 0.95f;
  // Side face - create only one
  const auto sideFaceVerts = makePyramidFace(radius, baseY, apexY);
  GLuint vaoSide = setupGeometry(sideFaceVerts);
  // Base
  const auto baseVerts = makePyramidBase(radius, baseY);
  const int pyramidBaseVertCount = (int)baseVerts.size();
  GLuint vaoBase = setupGeometry(baseVerts);

  /* Program */
  GLint uMVP = glGetUniformLocation(program, "MVP");
  GLint uIsHelix = glGetUniformLocation(program, "isHelix");
  GLint uTotalPts = glGetUniformLocation(program, "totalPoints");

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.1f, 0.1f, 0.12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(45.f), 1200.f / 600.f, 0.1f, 50.f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.f, 1.f, 4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    // User rotations
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
      glUniform1i(uIsHelix, 0);

      // Draw base
      glBindVertexArray(vaoBase);
      glDrawArrays(GL_TRIANGLE_FAN, 0, pyramidBaseVertCount);

      // Draw sides - copy return of makePyramidFace and rotate
      for (int i = 0; i <= N; ++i)
      {
        float ang = i * 2.0f * M_PI / N;
        glm::mat4 faceModel = model;
        faceModel = glm::rotate(faceModel, -ang, glm::vec3(0.f, 1.f, 0.f));

        glm::mat4 faceMVP = proj * view * faceModel;
        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(faceMVP));

        glBindVertexArray(vaoSide);
        glDrawArrays(GL_TRIANGLES, 0, 3);
      }
    }
    // Refresh
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Clleanp
  glfwTerminate();
  return 0;
}
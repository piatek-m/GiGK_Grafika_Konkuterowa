#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 color;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        color = aColor;
    }
)";

const char* fragmentSrc = R"(
    #version 330 core
    in vec3 color;
    out vec4 fragColor;
    void main() {
        fragColor = vec4(color, 1.0);
    }
)";

float vertices[] = {
    -0.4f, -0.5f,   0.6f, 0.3f, 0.1f,
     0.4f, -0.5f,   0.6f, 0.3f, 0.1f,
     0.4f,  0.1f,   0.6f, 0.3f, 0.1f,
    -0.4f,  0.1f,   0.6f, 0.3f, 0.1f,

    -0.5f,  0.1f,   0.8f, 0.1f, 0.1f,
     0.5f,  0.1f,   0.8f, 0.1f, 0.1f,
     0.0f,  0.6f,   0.9f, 0.2f, 0.2f,

    -0.08f, -0.5f,   0.3f, 0.15f, 0.05f,
     0.08f, -0.5f,   0.3f, 0.15f, 0.05f,
     0.08f, -0.15f,  0.3f, 0.15f, 0.05f,
    -0.08f, -0.15f,  0.3f, 0.15f, 0.05f,

    -0.32f, -0.15f,  0.6f, 0.85f, 0.95f,
    -0.15f, -0.15f,  0.6f, 0.85f, 0.95f,
    -0.15f,  0.05f,  0.6f, 0.85f, 0.95f,
    -0.32f,  0.05f,  0.6f, 0.85f, 0.95f,

     0.15f, -0.15f,  0.6f, 0.85f, 0.95f,
     0.32f, -0.15f,  0.6f, 0.85f, 0.95f,
     0.32f,  0.05f,  0.6f, 0.85f, 0.95f,
     0.15f,  0.05f,  0.6f, 0.85f, 0.95f,
};

unsigned int indices[] = {
    0,  1,  2,    2,  3,  0,
    4,  5,  6,
    7,  8,  9,    9, 10,  7,
    11, 12, 13,  13, 14, 11,
    15, 16, 17,  17, 18, 15,
};

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader error: " << log << "\n";
    }
    return shader;
}

GLuint createProgram() {
    GLuint vs   = compileShader(GL_VERTEX_SHADER,   vertexSrc);
    GLuint fs   = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

void onResize(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Domek 2D", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);
    glewExperimental = GL_TRUE;
    glewInit();
    return window;
}

GLuint setupGeometry() {
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return VAO;
}

int main() {
    GLFWwindow* window = initWindow();
    GLuint program     = createProgram();
    GLuint VAO         = setupGeometry();

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

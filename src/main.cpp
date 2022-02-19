#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

const static int SHADER_LOG_LEN = 512;
const static int PROGRAM_LOG_LEN = SHADER_LOG_LEN;

const float vertices[] = {
    // location         // color
    -0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
    0.5f, -0.5f, 0.0f,  0.f, 1.f, 0.f,
    0.0f, 0.5f, 0.0f,   0.f, 0.f, 1.f
};

const float vertices2[] = {
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
};

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void createVaoVbo(unsigned int* vao, unsigned int *vbo, const float * vertexData, const int count) {
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertexData, GL_STATIC_DRAW);

    std::cout << "size: " << sizeof(*vertexData) << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Uniform(int program) {
    float timeValue = glfwGetTime();
    float green = (sin(timeValue) / 2.0f) + 0.5f;
    int ucolor = glGetUniformLocation(program, "fragColor");
    glUseProgram(program);
    if (ucolor != -1)
        glUniform4f(ucolor, 0.f, green, 0.f, 1.f);
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Init GLAD failed..." << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    std::cout << "This machine support vertex attributes'count: " << maxVertexAttribs << std::endl;

    const char *vsPath = "../shader/triangle.vs";
    const char *fsPath = "../shader/triangle.fs";

    Shader *s = new Shader(vsPath, fsPath);

    // VAO, VBO, EBO
    unsigned int VAO[2], VBO[2];

    createVaoVbo(&VAO[0], &VBO[0], vertices, sizeof(vertices)/sizeof(float));

    createVaoVbo(&VAO[1], &VBO[1], vertices2, sizeof(vertices2)/sizeof(float));

    for (int i = 0; i < 2; i++) {
        std::cout << "Outside vao: " << VAO[i] << " vbo: " << VBO[i] << std::endl;
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        // glClear(GL_COLOR_BUFFER_BIT);
        // Uniform(shaderProgram);

        s->use();
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Uniform(shaderProgramYellow);
        // glBindVertexArray(VAO[1]);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(sizeof(VAO), VAO);
    glDeleteBuffers(sizeof(VBO), VBO);
    // glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


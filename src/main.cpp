#include "Shader.h"
#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

const static int SHADER_LOG_LEN = 512;
const static int PROGRAM_LOG_LEN = SHADER_LOG_LEN;

const float vertices[] = {
    // location         // texture
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const int indices[] = {
    0, 1, 3,
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

    const int vertexAttribCount = 5;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

unsigned int createEBO(const int*eboData, const int count) {
    unsigned int ebo;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, eboData, GL_STATIC_DRAW);

    return ebo;
}

void Uniform(int program) {
    float timeValue = glfwGetTime();
    float green = (sin(timeValue) / 2.0f) + 0.5f;
    int ucolor = glGetUniformLocation(program, "fragColor");
    glUseProgram(program);
    if (ucolor != -1)
        glUniform4f(ucolor, 0.f, green, 0.f, 1.f);
}

void mvp(Shader& s, int screenWidth, int screenHeight) {
    glm::vec4 vec(1.f, 0.f, 0.f, 1.f);
    glm::mat4 m = glm::mat4(1.f);
    glm::mat4 v = glm::mat4(1.f);
    glm::mat4 p = glm::mat4(1.f);

    // m = glm::rotate(m, (float)glfwGetTime() * glm::radians(50.f), glm::vec3(0.5f, 1.f, 0.f));
    v = glm::translate(v, glm::vec3(0.f, 0.f, -3.f));
    p = glm::perspective(glm::radians(45.f), static_cast<float>(screenWidth/screenHeight), 0.1f, 100.f);

    // s.setMat4f("model", m);
    s.setMat4f("view", v);
    s.setMat4f("projection", p);

    // std::cout << "location: " << transformLoc << " matrix: " << glm::to_string(trans) << std::endl; 
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

    const int screenWidth = 800, screenHeight = 600;

    glViewport(0, 0, screenWidth, screenWidth);

    unsigned int texID = loadTexture("../texture/container.jpg", false);
    unsigned int texID2 = loadTexture("../texture/awesomeface.png", true);

    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    std::cout << "This machine support vertex attributes'count: " << maxVertexAttribs << std::endl;

    const char *vsPath = "../shader/matrix.vs";
    const char *fsPath = "../shader/matrix.fs";

    Shader s(vsPath, fsPath);

    // VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;

    createVaoVbo(&VAO, &VBO, vertices, sizeof(vertices)/sizeof(float));

    EBO = createEBO(indices, sizeof(indices)/sizeof(int));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    s.use();
    s.setInt("ourTexture", 0);
    s.setInt("ourTexture2", 1);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        Uniform(s.getProgramID());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texID2);

        s.use();
        // IMPORTANT! MUST glUseProgram first before glGetUniformLocation
        mvp(s, screenWidth, screenHeight);

        glBindVertexArray(VAO);
        const glm::vec3 cubePositions[] = {
          glm::vec3( 0.0f,  0.0f,  0.0f), 
          glm::vec3( 2.0f,  5.0f, -15.0f), 
          glm::vec3(-1.5f, -2.2f, -2.5f),  
          glm::vec3(-3.8f, -2.0f, -12.3f),  
          glm::vec3( 2.4f, -0.4f, -3.5f),  
          glm::vec3(-1.7f,  3.0f, -7.5f),  
          glm::vec3( 1.3f, -2.0f, -2.5f),  
          glm::vec3( 1.5f,  2.0f, -2.5f), 
          glm::vec3( 1.5f,  0.2f, -1.5f), 
          glm::vec3(-1.3f,  1.0f, -1.5f)  
        };
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.f, 0.3f, 0.5f));
            s.setMat4f("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
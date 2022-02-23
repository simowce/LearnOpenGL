#include "Shader.h"
#include "common.h"

const static int SHADER_LOG_LEN = 512;
const static int PROGRAM_LOG_LEN = SHADER_LOG_LEN;

const float vertices[] = {
    // location        // color       // texture
    0.5f, 0.5f, 0.f,   1.f, 0.f, 0.f, 1.f, 1.f,
    0.5f, -0.5f, 0.f,  0.f, 1.f, 0.f, 1.f, 0.f,
    -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
    -0.5f, 0.5f, 0.f,  1.f, 1.f, 0.f, 0.f, 1.f
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

    std::cout << "size: " << sizeof(*vertexData) << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
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

    unsigned int texID = loadTexture("../texture/container.jpg", false);
    unsigned int texID2 = loadTexture("../texture/awesomeface.png", true);

    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    std::cout << "This machine support vertex attributes'count: " << maxVertexAttribs << std::endl;

    const char *vsPath = "../shader/triangle.vs";
    const char *fsPath = "../shader/triangle.fs";

    Shader s(vsPath, fsPath);

    // VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;

    createVaoVbo(&VAO, &VBO, vertices, sizeof(vertices)/sizeof(float));

    EBO = createEBO(indices, sizeof(indices)/sizeof(int));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    s.use();
    s.setInt("ourTexture", 0);
    s.setInt("ourTexture2", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        // glClear(GL_COLOR_BUFFER_BIT);
        Uniform(s.getProgramID());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texID2);

        s.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
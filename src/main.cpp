#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const static int SHADER_LOG_LEN = 512;
const static int PROGRAM_LOG_LEN = SHADER_LOG_LEN;

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
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

const char *vertexShaderSource = "#version 330 core\n"
    "layout(location=0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int shader(int type, const char *source) {
    unsigned int sid;
    int ret;

    sid = glCreateShader(type);
    glShaderSource(sid, 1, &source, NULL);
    glCompileShader(sid);
    glGetShaderiv(sid, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        char log[SHADER_LOG_LEN];
        glGetShaderInfoLog(sid, SHADER_LOG_LEN, NULL, log);
        std::cout << "shader compile error! type: " << type << " reason: " << log << std::endl;
    }

    return !ret ? -1 : sid;
}

int program(int vs, int fs) {
    unsigned int pid;
    int ret;

    pid = glCreateProgram();
    glAttachShader(pid, vs);
    glAttachShader(pid, fs);
    glLinkProgram(pid);
    glGetShaderiv(pid, GL_LINK_STATUS, &ret);
    if (!ret) {
        char log[PROGRAM_LOG_LEN];
        glGetProgramInfoLog(pid, PROGRAM_LOG_LEN, NULL, log);
        std::cout << "program link error! ID: " << pid << " reason: " << log << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);

    return pid;
}

void createVaoVbo(unsigned int* vao, unsigned int *vbo, const float * vertexData, const int count) {
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);

    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertexData, GL_STATIC_DRAW);

    std::cout << "size: " << sizeof(*vertexData) << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
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

    // vertex shader
    unsigned int vertexShader = shader(GL_VERTEX_SHADER, vertexShaderSource);

    // fragment shader
    unsigned int fragmentShader = shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // yellow fragment shader
    unsigned int fragmentShaderYellow = shader(GL_FRAGMENT_SHADER, fragmentShaderSourceYellow);

    // program
    unsigned int shaderProgram = program(vertexShader, fragmentShader);

    unsigned int shaderProgramYellow = program(vertexShader, fragmentShaderYellow);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(sizeof(VAO), VAO);
    glDeleteBuffers(sizeof(VBO), VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

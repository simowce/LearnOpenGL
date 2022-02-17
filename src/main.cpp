#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const static int SHADER_LOG_LEN = 512;
const static int PROGRAM_LOG_LEN = SHADER_LOG_LEN;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
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

const char *fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
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

    // program
    unsigned int shaderProgram = program(vertexShader, fragmentShader);

    // VAO, VBO, EBO
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        // glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

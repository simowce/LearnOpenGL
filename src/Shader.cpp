#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

Shader::Shader(const char *vsPath, const char *fsPath)
{
    std::string vsStr, fsStr;
    std::ifstream vsFile, fsFile;
    const char *vsCode, *fsCode;

    vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream vsStream, fsStream;

        vsFile.open(vsPath);
        fsFile.open(fsPath);

        vsStream << vsFile.rdbuf();
        fsStream << fsFile.rdbuf();

        vsFile.close();
        fsFile.close();

        vsStr = vsStream.str();
        fsStr = fsStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR! read shader file failed: " << e.what() << std::endl;
    }

    vsCode = vsStr.c_str();
    fsCode = fsStr.c_str();

    mVertexShaderID = initShader(vsPath, vsCode, GL_VERTEX_SHADER);
    mFragmentShaderID = initShader(fsPath, fsCode, GL_FRAGMENT_SHADER);
    mProgramID = initProgram(mVertexShaderID, mFragmentShaderID);

    glDeleteShader(mVertexShaderID);
    glDeleteShader(mFragmentShaderID);
}

unsigned int Shader::initShader(const char *shaderPath, const char *shaderCode, int shaderType)
{
    unsigned int sid;
    int ret = 0;

    sid = glCreateShader(shaderType);
    glShaderSource(sid, 1, &shaderCode, NULL);
    glCompileShader(sid);
    glGetShaderiv(sid, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        char log[LOG_LEN];
        glGetShaderInfoLog(sid, LOG_LEN, NULL, log);
        std::cout << "ERROR! shader init failed! shader path: " << shaderPath
                  << " log: " << log << std::endl;
    }

    return sid;
}

unsigned int Shader::initProgram(int vs, int fs)
{
    unsigned int progId;
    int ret = 0;

    progId = glCreateProgram();
    glAttachShader(progId, vs);
    glAttachShader(progId, fs);
    glLinkProgram(progId);
    glGetProgramiv(progId, GL_LINK_STATUS, &ret);
    if (!ret) {
        char log[LOG_LEN];
        glGetProgramInfoLog(progId, LOG_LEN, NULL, log);
        std::cout << "ERROR! Program link error! vertex shader ID: " << vs << " fragment shader ID: " << fs
                  << " log: " << log;
    }

    return progId;
}

void Shader::use()
{
    glUseProgram(mProgramID);
}

unsigned int Shader::getProgramID()
{
    return mProgramID;
}

void Shader::setInt(const char *name, int v)
{
    glUniform1i(glGetUniformLocation(mProgramID, name), (int)v);
}

void Shader::setBool(const char *name, bool v)
{
    glUniform1i(glGetUniformLocation(mProgramID, name), (int)v);
}

void Shader::setFloat(const char *name, float v)
{
    glUniform1f(glGetUniformLocation(mProgramID, name), (float)v);
}

Shader::~Shader()
{
    glDeleteProgram(mProgramID);
}

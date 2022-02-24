#ifndef _SHADER_H_
#define _SHADER_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char *vs, const char *fs);

    void use();

    unsigned int getProgramID();

    void setInt(const char *name, int v);

    void setBool(const char *name, bool v);

    void setFloat(const char *name, float v);

    void setMat4f(const char *name, glm::mat4 m);

    ~Shader();

private:
    char *pVS;
    char *pFS;

    unsigned int initShader(const char *path, const char *code, int shaderType);
    unsigned int initProgram(int vs, int fs);

    unsigned int mVertexShaderID;
    unsigned int mFragmentShaderID;
    unsigned int mProgramID;

    const int LOG_LEN = 512;
};

#endif

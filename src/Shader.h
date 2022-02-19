#ifndef _SHADER_H_
#define _SHADER_H_

class Shader {
public:
    Shader(const char *vs, const char *fs);

    void use();

    void setInt(const char *name, int v);

    void setBool(const char *name, bool v);

    void setFloat(const char *name, float v);

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

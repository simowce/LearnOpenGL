#define STB_IMAGE_IMPLEMENTATION
#include "common.h"
#include "stb_image.h"

unsigned int loadTexture(const char *texPath, bool isPNG)
{
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texPath, &width, &height, &nrChannels, 0);
    if (data) {
        int colorMode = isPNG ? GL_RGBA : GL_RGB;
        fmt::print("Picture load successfully. path: {} w: {} h: {} channel: {}\n", texPath, width, height, nrChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture..." << std::endl;
    }
    stbi_image_free(data);

    return texture;
}
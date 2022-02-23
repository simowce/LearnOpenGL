#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fmt/core.h"

unsigned int loadTexture(const char *texPath, bool isPNG);

#endif
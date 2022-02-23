cmake_minimum_required(VERSION 3.15)

project(LearnOpenGL)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/fmt)

# /usr/bin/ld: /usr/lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/Scrt1.o: in function `_start':
# (.text+0x24): undefined reference to `main'
# /usr/bin/ld: CMakeFiles/opengl.dir/src/glad.c.o: in function `open_gl':
# glad.c:(.text+0x3b): undefined reference to `dlopen'
# https://stackoverflow.com/questions/956640/linux-c-error-undefined-reference-to-dlopen/22996058
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

add_executable(opengl ${SOURCE_PATH}/main.cpp
                      ${SOURCE_PATH}/Shader.cpp
                      ${SOURCE_PATH}/Texture.cpp
                      ${SOURCE_PATH}/glad.c)

# main.cpp:(.text+0x9): undefined reference to `glfwInit'
# /usr/bin/ld: main.cpp:(.text+0x18): undefined reference to `glfwWindowHint'
# /usr/bin/ld: main.cpp:(.text+0x27): undefined reference to `glfwWindowHint'
# /usr/bin/ld: main.cpp:(.text+0x36): undefined reference to `glfwWindowHint'
# collect2: error: ld returned 1 exit status
# https://blog.csdn.net/weixin_44694952/article/details/105183076
target_link_libraries(opengl glfw fmt::fmt)

include_directories(${CMAKE_SOURCE_DIR}/include)

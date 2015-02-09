#ifndef _GL_INTERFACE_HPP_
#define _GL_INTERFACE_HPP_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#define PI 3.14159265359

void initGL(void);
void error_callback(int error, const char * description);
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

#endif

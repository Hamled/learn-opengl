#ifndef __INCLUDE__APP_H__
#define __INCLUDE__APP_H__
#include <iostream>
#include <GLFW/glfw3.h>

namespace {

void glfw_error_callback(int error, const char* description);
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int with_window(std::function<  int(GLFWwindow*) > block);

} // namespace

#endif

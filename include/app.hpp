#ifndef __INCLUDE__APP_H__
#define __INCLUDE__APP_H__
#include <iostream>
#include <GLFW/glfw3.h>

namespace {

void glfw_error_callback(int error, const char* description);
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
template<typename WindowBlock> int with_window(WindowBlock &&block) {
  // Initialize GLFW
  if(!glfwInit()) {
    std::cout << "Unable to initialize GLFW!" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "GLFW initialized successfully." << std::endl;

  // Configure our desired context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // Create the window
  const auto window = glfwCreateWindow(640, 480, "Learn OpenGL", nullptr, nullptr);
  if(window == nullptr) {
    std::cout << "Unable to create OpenGL context!" << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Run our code with the window
  const auto res = block(window);

  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "GLFW terminated successfully." << std::endl;

  return res;
}

} // namespace

#endif

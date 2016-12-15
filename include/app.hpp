#ifndef __INCLUDE__APP_H__
#define __INCLUDE__APP_H__
#include <stdexcept>
#include <iostream>
#include <GLFW/glfw3.h>

namespace {

void glfw_error_callback(int error, const char* description);
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
template<typename WindowBlock> void with_window(WindowBlock &&block) {
  // Initialize GLFW
  if(!glfwInit()) {
    throw std::runtime_error("Unable to initialize GLFW!");
  }
  std::cout << "GLFW initialized successfully." << std::endl;


  // Configure our desired context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // Create the window
  const auto cleanup = [](const auto window) {
    if(window) glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "GLFW terminated successfully." << std::endl;
  };

  const std::unique_ptr<GLFWwindow, decltype(cleanup)> window(
    glfwCreateWindow(640, 480, "Learn OpenGL", nullptr, nullptr),
    cleanup
  );

  if(!window) {
    cleanup(window.get());
    throw std::runtime_error("Unable to create OpenGL context!");
  }

  // Run our code with the window
  block(window.get());
}

GLuint buildVertShader();
GLuint buildFragShader();

} // namespace

#endif

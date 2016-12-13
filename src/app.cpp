#include <iostream>
#include <GLFW/glfw3.h>

void glfw_error_callback(int error, const char* description);

int main() {
  glfwSetErrorCallback(glfw_error_callback);

  if(!glfwInit()) {
    std::cout << "Unable to initialize GLFW!" << std::endl;
    return 1;
  }

  std::cout << "GLFW initialized successfully." << std::endl;
  glfwTerminate();
  return 0;
}

void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

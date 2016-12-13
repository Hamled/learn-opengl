#include <iostream>
#include <GLFW/glfw3.h>

int main() {
  if(!glfwInit()) {
    std::cout << "Unable to initialize GLFW!" << std::endl;
    return 1;
  }

  std::cout << "GLFW initialized successfully." << std::endl;
  glfwTerminate();
  return 0;
}

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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(640, 480, "Learn OpenGL", NULL, NULL);
  if(!window) {
    std::cout << "Unable to create OpenGL context!" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

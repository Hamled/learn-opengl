#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void glfw_error_callback(int error, const char* description);

int main() {
  glfwSetErrorCallback(glfw_error_callback);

  // Initialize GLFW
  if(!glfwInit()) {
    std::cout << "Unable to initialize GLFW!" << std::endl;
    return 1;
  }
  std::cout << "GLFW initialized successfully." << std::endl;

  // Configure our desired context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create the window
  GLFWwindow* window = glfwCreateWindow(640, 480, "Learn OpenGL", NULL, NULL);
  if(!window) {
    std::cout << "Unable to create OpenGL context!" << std::endl;
    glfwTerminate();
    return 1;
  }

  // Use the window's OpenGL context
  glfwMakeContextCurrent(window);

  // Load all Open GL functionality w/ glad
  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Unable to create OpenGL context!" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  // Setup the viewport
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Loop until the window should close
  while(!glfwWindowShouldClose(window)) {
    // We must wait for events or this
    // spinloop will prevent anything from happening
    glfwWaitEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

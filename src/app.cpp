#include <iostream>
#include <glad/glad.h>
#include <app.hpp>

int main() {
  glfwSetErrorCallback(glfw_error_callback);

  const auto res = with_window([](GLFWwindow* window) -> int {
    // Use the window's OpenGL context
    glfwMakeContextCurrent(window);

    // Load all Open GL functionality w/ glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      std::cout << "Unable to create OpenGL context!" << std::endl;
      return EXIT_FAILURE;
    }

    // Listen for key events
    glfwSetKeyCallback(window, glfw_key_callback);

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

    return EXIT_SUCCESS;
  });

  std::exit(res);
}

namespace {
void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
} // namespace

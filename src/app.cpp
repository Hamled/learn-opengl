#include <stdexcept>
#include <iostream>
#include <glad/glad.h>
#include <app.hpp>

namespace {

extern "C" int main() {
  glfwSetErrorCallback(glfw_error_callback);

  try {
    with_window([](auto window) {
      // Use the window's OpenGL context
      glfwMakeContextCurrent(window);

      // Load all Open GL functionality w/ glad
      if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Unable to create OpenGL context!");
      }

      // Listen for key events
      glfwSetKeyCallback(window, glfw_key_callback);

      // Setup the viewport
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      glViewport(0, 0, width, height);

      // Loop until the window should close
      while(!glfwWindowShouldClose(window)) {
        // Handle any events that have happened since the last frame
        glfwPollEvents();

        // Render the new frame
        //   Start off by clearing the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //   Next render a single triangle
        //     First create and fill a vertex buffer
        const GLuint VBO = 0;
        glGenBuffers(1, const_cast<GLuint*>(&VBO));
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        const GLfloat vertices[] = {
          -0.5f, -0.5f, 0.f,
           0.5f, -0.5f, 0.f,
           0.5f,  0.5f, 0.f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Display the new frame
        glfwSwapBuffers(window);
      }
    });
  } catch(const std::exception &e) {
    std::cout << "Exiting with failure: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
} // namespace

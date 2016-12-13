#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int with_window(std::function<  int(GLFWwindow*) > block);
static void glfw_error_callback(int error, const char* description);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

void glfw_error_callback(int error, const char* description) {
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int with_window(std::function< int(GLFWwindow*) > block) {
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

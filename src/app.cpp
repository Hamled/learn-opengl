#include <stdexcept>
#include <iostream>
#include <glad/glad.h>
#include <app.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

      // Create and compile our shaders and shader program
      const GLuint vertShader = buildVertShader();
      const GLuint fragShader = buildFragShader();
      const GLuint shaderProg = buildShaderProg(vertShader, fragShader);
      // Delete the shaders since they've been linked
      glDeleteShader(vertShader);
      glDeleteShader(fragShader);

      // Get a handle to the uniform variable in our shader program
      const GLint timeUniLoc = glGetUniformLocation(shaderProg, "time");

      // Create and configure our vertex buffer and array objects
      GLuint VAO = 0, VBO = 0, EBO = 0;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
        // First fill the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        const GLfloat vertices[] = {
          // Position        // Color delta
          -0.5f, -0.5f, 0.f,  0.2f, -0.1f, 1.f,
           0.5f, -0.5f, 0.f, -0.1f,  0.2f, 2.f,
          -0.5f,  0.5f, 0.f,  0.1f,  0.2f, 2.f,
           0.5f,  0.5f, 0.f, -0.2f, -0.1f, 1.f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // And setup our element indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        const GLuint indices[] = {
          2, 3, 0, 1 // Both triangles
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Then configure the vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
      glBindVertexArray(NULL);


      // Loop until the window should close
      while(!glfwWindowShouldClose(window)) {
        // Handle any events that have happened since the last frame
        glfwPollEvents();

        // Render the new frame
        //   Start off by clearing the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //   Next render a single triangle
        //     Setup the shader program
        glUseProgram(shaderProg);
        glUniform1f(timeUniLoc, static_cast<GLfloat>(glfwGetTime()));

        //     Use the vertex array
        glBindVertexArray(VAO);

        //     Actually render
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

        //     Unbind the vertex array
        glBindVertexArray(NULL);

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

GLuint buildVertShader() {
  const auto vertShader = glCreateShader(GL_VERTEX_SHADER);
  const auto vs_glsl = R"glsl(
  #version 330 core

  layout (location = 0) in vec3 pos;
  layout (location = 1) in vec3 colorDelta;

  out vec3 vertColor;

  void main() {
    gl_Position = vec4(pos, 1.0f);
    vertColor = colorDelta;
  }
  )glsl";

  glShaderSource(vertShader, 1, &vs_glsl, NULL);
  glCompileShader(vertShader);

  auto success = 0;
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    GLchar msg[512] = {0};
    glGetShaderInfoLog(vertShader, 511, NULL, msg);
    std::cout << "Unable to compile vertex shader: " << msg << std::endl;
    throw std::runtime_error("Error building shaders");
  }

  return vertShader;
}

GLuint buildFragShader() {
  const auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const auto fg_glsl = R"glsl(
  #version 330 core

  in vec3 vertColor;

  out vec4 color;

  uniform float time;

  void main() {
    float red   = (sin(time) / 2) + 0.5f;
    float green = (cos(time) / 2) + 0.5f;
    color = vec4(red + vertColor.x, green + vertColor.y, 0.f + vertColor.z, 1.0f);
  }
  )glsl";

  glShaderSource(fragShader, 1, &fg_glsl, NULL);
  glCompileShader(fragShader);

  auto success = 0;
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    GLchar msg[512] = {0};
    glGetShaderInfoLog(fragShader, 511, NULL, msg);
    std::cout << "Unable to compile fragment shader: " << msg << std::endl;
    throw std::runtime_error("Error building shaders");
  }

  return fragShader;
}

GLuint buildShaderProg(const GLuint vertShader, const GLuint fragShader) {
  const auto shaderProg = glCreateProgram();

  glAttachShader(shaderProg, vertShader);
  glAttachShader(shaderProg, fragShader);
  glLinkProgram(shaderProg);

  auto success = 0;
  glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);

  if(!success) {
    GLchar msg[512] = {0};
    glGetShaderInfoLog(shaderProg, 511, NULL, msg);
    std::cout << "Unable to link shader program: " << msg << std::endl;
    throw std::runtime_error("Error building shaders");
  }

  return shaderProg;
}

GLuint loadTexture(const char* filename) {
  // Load the image data from a file
  const auto imageCleanup = [](const auto image) {
    stbi_image_free(image);
  };

  const int req_channels = 3; // RGB
  int width, height, channels;
  const std::unique_ptr<stbi_uc, decltype(imageCleanup)> image(
    stbi_load(filename, &width, &height, &channels, req_channels),
    imageCleanup
  );

  if(!image) {
    std::cout << "Unable to load image '" << filename << "' "
              << "for texture: " << stbi_failure_reason() << std::endl;
    throw std::runtime_error("Error loading texture");
  }

  // Use the image data to create a texture
  GLuint texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.get());

  // Generate all mipmap levels for the texture
  glGenerateMipmap(GL_TEXTURE_2D);

  // We're done with the texture for now
  glBindTexture(GL_TEXTURE_2D, 0);

  bool gotError = false;
  GLenum glError = GL_NO_ERROR;
  while((glError = glGetError()) != GL_NO_ERROR) {
    std::cout << "Unable to create texture from image '" << filename << "' "
              << "due to GL error: " << glError << std::endl;
    gotError = true;
  }

  if(gotError) {
    throw std::runtime_error("Error loading texture");
  }

  return texture;
}

} // namespace

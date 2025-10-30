#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// Whenever the window changes in size, GLFW calls this function
// and fills in the proper arguments for you to process.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Input: ESC close the window
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

int main() {
  // Initialize GLFW
  glfwInit();

  // It lock to the version 3 of opengl
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // We want to use the core profile (smaller subset)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Creation of a window object
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Silksong", NULL, NULL);

  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // We call GLAD before everything else
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return -1;
  }

  // Tell opengl the size of the rendering
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // Callback function
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // vertices of the triangles, the z is 0 to make it 2D
  float vertices[] = {
      // first triangle
      -0.9f, -0.5f, 0.0f, // left
      -0.0f, -0.5f, 0.0f, // right
      -0.45f, 0.5f, 0.0f, // top
                          // second triangle
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f   // top
  };

  // VERTEX ARRAY OBJECT (VAO): contains the vertex of the objects
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Buffer Object (VBO): necessary for sending data to the GPU
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // To bind our buffer to GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // To copy our vertices in memory
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create a shader object
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Compile the shader
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check for failure in shader's compilation
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    printf("%s", infoLog);
  }

  // Second shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    printf("%s", infoLog);
  }

  // Create a shader program
  unsigned int shaderProgram = glCreateProgram();

  // Attach the shaders to the program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n");
    printf("%s", infoLog);
  }

  // After the linking we don't need the shader anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Tell opengl how interpret the vertex data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Clear at the start of every iteration
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDrawArrays(GL_TRIANGLES, 3, 3);

    // Check for input
    processInput(window);
    // Swap the color of the buffer
    glfwSwapBuffers(window);

    // CHeck for events
    glfwPollEvents();
  }

  // Free all resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();

  return 0;
}

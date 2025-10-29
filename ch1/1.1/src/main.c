#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // Clear at the start of every iteration
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Check for input
    processInput(window);
    // Swap the color of the buffer
    glfwSwapBuffers(window);

    // CHeck for events
    glfwPollEvents();
  }

  // Free all resources
  glfwTerminate();

  return 0;
}

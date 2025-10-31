#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/image_implementation.h"
#include "../include/shader_s.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow *initializeGLFW();
bool initializeGLAD();
unsigned int setupVertexData();
unsigned int createTexture(const char *texturePath, bool flipVertical = false);
void renderScene(GLFWwindow *window, unsigned int VAO, unsigned int texture1,
                 unsigned int texture2, Shader &shader);
void cleanup(unsigned int VAO, unsigned int VBO, unsigned int EBO,
             unsigned int texture1, unsigned int texture2);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global variables for shader and textures
Shader *ourShader = nullptr;
unsigned int texture1, texture2;

int main() {
  // Initialize GLFW and create window
  GLFWwindow *window = initializeGLFW();
  if (!window) {
    return -1;
  }

  // Initialize GLAD
  if (!initializeGLAD()) {
    return -1;
  }

  // Initialize shader
  ourShader = new Shader("texture.vs", "texture.fs");

  // Setup vertex data and buffers
  unsigned int VAO = setupVertexData();

  // Load textures
  texture1 = createTexture("texture.jpg");
  texture2 = createTexture("awesomeface.png", true);

  // Set texture units in shader
  ourShader->use();
  ourShader->setInt("texture1", 0);
  ourShader->setInt("texture2", 1);

  // Main render loop
  renderScene(window, VAO, texture1, texture2, *ourShader);

  // Cleanup
  cleanup(VAO, 0, 0, texture1, texture2);
  delete ourShader;

  glfwTerminate();
  return 0;
}

GLFWwindow *initializeGLFW() {
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

bool initializeGLAD() {
  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }
  return true;
}

unsigned int setupVertexData() {
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // NOTE: Removed color data to match the reference code structure
  float vertices[] = {
      // positions          // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute - 5 * sizeof(float) stride instead of 8
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute - only 2 texture coordinates
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return VAO;
}

unsigned int createTexture(const char *texturePath, bool flipVertical) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image
  int width, height, nrChannels;

  if (flipVertical) {
    stbi_set_flip_vertically_on_load(true);
  }

  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data) {
    // Handle different texture formats correctly
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture: " << texturePath << std::endl;
  }

  stbi_image_free(data);
  stbi_set_flip_vertically_on_load(false); // Reset to default

  return texture;
}

void renderScene(GLFWwindow *window, unsigned int VAO, unsigned int texture1,
                 unsigned int texture2, Shader &shader) {
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Create transformation - same as reference
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(),
                            glm::vec3(0.0f, 0.0f, 1.0f));

    // Render
    shader.use();
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void cleanup(unsigned int VAO, unsigned int VBO, unsigned int EBO,
             unsigned int texture1, unsigned int texture2) {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

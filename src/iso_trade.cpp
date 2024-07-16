#include <stdio.h>

// #define GLEW_USE_STATIC_LIBS
// #define GLEW_STATIC
// #define GLFW_STATIC

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Resolutions (16/9 ratio)
#define RESOLUTION_WIDTH 1600
#define RESOLUTION_HEIGHT 900
#define TITLE_BAR "Iso Trade"

// Static pointers to stuff
static GLFWwindow* window = NULL;


/**
 * Main function - some code taken from: http://github.com/opengl-tutorials/ogl/blob/master/tutorial01_first_window/tutorial01.cpp
 */

int main(int argc, char** argv) {
  printf("Hello, iso-trade!\n");

  // Initialize GLFW
  if (!glfwInit()) {
    printf("Failed to initialize GLFW\n");
    return -1;
  }

  // Hints for GLFW
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No old OpenGL

  window = glfwCreateWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, TITLE_BAR, NULL, NULL);
  if (window == NULL) {
    printf("Failed to open GLFW window\n");
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // Main loop
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw nothing yet

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  printf("All went well, goodbye!\n");
  return 0;
}

#include <stdio.h>

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
 * Shaders code as static strings
 */

const char* default_shader_vertex =
R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

  fragmentColor = vertexColor;
}
)";

const char* default_shader_fragment =
R"(
#version 330 core

in vec3 fragmentColor;

out vec3 color;

void main() {
  color = fragmentColor;
}
)";

/**
 * Shader loading function
 */

GLuint load_shaders(const char* vertex_source, const char* fragment_source) {
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  GLuint program_id = glCreateProgram();

  GLint result = GL_FALSE;
  int info_log_length;
  
  // Compile vertex shader
  glShaderSource(vertex_shader_id, 1, &vertex_source, NULL);
  glCompileShader(vertex_shader_id);

  // Check vertex shader for errors
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    GLchar vertex_shader_error_message[512];
    glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
    printf("%s\n", &vertex_shader_error_message[0]);
  }

  // Compile fragment shader
  glShaderSource(fragment_shader_id, 1, &fragment_source, NULL);
  glCompileShader(fragment_shader_id);

  // Check fragment shader for errors
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    GLchar fragment_shader_error_message[512];
    glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_message[0]);
    printf("%s\n", &fragment_shader_error_message[0]);
  }

  // Link the program
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // Check the program
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    GLchar program_error_message[512];
    glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
    printf("%s\n", &program_error_message[0]);
  }

  // Cleanup - detach and delete shaders - they are no longer needed since they are now part of the program
  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  
  return program_id;
}

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

  // glewExperimental
  glewExperimental = true;

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");

    glfwTerminate();
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // Some enablement
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Initialize vertexarray
  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // Loading shaders into program
  GLuint program_id = load_shaders(default_shader_vertex, default_shader_fragment);

  // printf("program_id: %i", program_id);
  
  // Getting ID for uniforms
  GLuint mvp_matrix_id = glGetUniformLocation(program_id, "MVP");

  // CAMERA STUFF - PROJECTION MATRICIES
  // @TODO - move FOV and RATIO to different place, you know which is which, also near and far as inside parameters of game
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1, 100.0f);
  glm::mat4 view = glm::lookAt(
      glm::vec3(4, 3, -3), // Camera pos
      glm::vec3(0, 0, 0),  // Where to look at
      glm::vec3(0, 1, 0)   // Camera up vector
      );
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 MVP = projection * view * model;

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

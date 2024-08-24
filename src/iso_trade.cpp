#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Local engine includes
#include <camera/camera.hpp>

// Resolutions (16/9 ratio)
#define RESOLUTION_WIDTH 1600
#define RESOLUTION_HEIGHT 900
#define TITLE_BAR "Iso Trade"

// Static pointers to stuff
static GLFWwindow* window = NULL;

/**
 * Static data for colored cube
 */

static const GLfloat colored_cube_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
};

static const GLfloat colored_cube_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
};

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

  // Some enablement
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Loading shaders into program
  GLuint shader_program_id = load_shaders(default_shader_vertex, default_shader_fragment);
  
  // Getting ID for uniforms
  GLuint mvp_matrix_id = glGetUniformLocation(shader_program_id, "MVP");

  // CAMERA STUFF - PROJECTION MATRICIES
  // @TODO - move FOV and RATIO to different place, you know which is which, also near and far as inside parameters of game
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(
      glm::vec3(4, 3, -3), // Camera pos
      glm::vec3(0, 0, 0),  // Where to look at
      glm::vec3(0, 1, 0)   // Camera up vector
      );
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 MVP = projection * view * model;

  // Print out whole MVP matrix
  printf("MVP: %.6f %.6f %.6f %.6f\n", MVP[0][0], MVP[0][1], MVP[0][2], MVP[0][3]);
  printf("   : %.6f %.6f %.6f %.6f\n", MVP[1][0], MVP[1][1], MVP[1][2], MVP[1][3]);
  printf("   : %.6f %.6f %.6f %.6f\n", MVP[2][0], MVP[2][1], MVP[2][2], MVP[2][3]);
  printf("   : %.6f %.6f %.6f %.6f\n", MVP[3][0], MVP[3][1], MVP[3][2], MVP[3][3]);

  // Initialize vertexarray - VAO
  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // Creating buffers - VBO
  GLuint vertex_buffer;
  GLuint color_buffer;

  printf("colored_cube_vertex_buffer_data: %p\n", colored_cube_vertex_buffer_data);
  printf("colored_cube_color_buffer_data:  %p\n", colored_cube_color_buffer_data);
  printf("colored_cube_vertex_ sizeof:     %zu\n", sizeof(colored_cube_vertex_buffer_data));
  printf("colored_cube_color_  sizeof:     %zu\n", sizeof(colored_cube_color_buffer_data));

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colored_cube_vertex_buffer_data), colored_cube_vertex_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colored_cube_color_buffer_data), colored_cube_color_buffer_data, GL_STATIC_DRAW);

  // DEBUG info
  printf("mvp_matrix_id:     %i\n", mvp_matrix_id);
  printf("shader_program_id: %i\n", shader_program_id);
  printf("vertex_array_id:   %i\n", vertex_array_id);
  printf("vertex_buffer:     %i\n", vertex_buffer);
  printf("color_buffer:      %i\n", color_buffer);

  // Time variables
  double delta_time = 0.0f;

  // Main loop
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
    // Calculate delta time
    double frame_start_time = glfwGetTime();
    // printf("Frame start time: %f\n", frame_start_time);

    // Gather input

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT IS SUPER FUCKIN IMPORTANT HERE
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // printf("Main loop\n");

    // Drawing colored cube
    // 1. Use shader program
    // 2. "Send" data to uniform stuff in shader - in this case MVP matrix
    // 3. Here we need to do three things: enable vertex attrib array (connected to layouts in shader)
    //    bind corresponding buffer and using atrrib pointer setup layout of data
    //    this is a bit tricky, but one should get used to it in OpenGL
    // 4. Draw call - type of draw and how many primitives should be drawn
    //    number of primitives of course is on CPU side for dynamic stuff
    // 5. Disable vertex attrib arrays at the end of render calls
    // Shaders program
    glUseProgram(shader_program_id);
    
    // Push data to uniform
    glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &MVP[0][0]);

    // Enable data from buffer to shader - vertex
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
      0,         // attribute - matching layout in shader
      3,         // size
      GL_FLOAT,  // type
      GL_FALSE,  // normalized?
      0,         // stride
      nullptr  // array buffer offset
        );

    // Enable data from buffer to shader - color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(
      1,         // attribute - matching layout in shader
      3,         // size
      GL_FLOAT,  // type
      GL_FALSE,  // normalized?
      0,         // stride
      nullptr  // array buffer offset
        );

    // Draw call
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12 triangles, each 3 indices

    // Disable attribs
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    // End of frame time
    double frame_end_time = glfwGetTime();
    delta_time = frame_end_time - frame_start_time;
    // printf("Delta time: %f\n", delta_time);
  }

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertex_buffer);
  glDeleteBuffers(1, &color_buffer);
  glDeleteProgram(shader_program_id);
  glDeleteVertexArrays(1, &vertex_array_id);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  printf("All went well, goodbye!\n");
  return 0;
}

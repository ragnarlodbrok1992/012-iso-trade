#ifndef _H_CAMERA
#define _H_CAMERA

#include <glm/glm.hpp> // TODO I don't know if this won't fuck up includes in main.

/*
 * What we mean by camera is a view matrix of MVP uniform in shader.
 * We modify it to get a position of camera that we want and other factors.
 * Moving up and down will be done twofold TODO: in vector normal to the game plane and up-down according to the
 * pitch.
 * Yaw can be done indefinitely, but pitch only 90 degrees up and down. No rolling, we won't fly any planes in this game.
 */

typedef struct {
  glm::vec3 camera_pos;
  glm::vec3 camera_front; // Unit vector point in the direction where camera is looking
  glm::vec3 camera_up_vector; // Should be 0, 1, 0 for the whole game, we do not roll ever.
  glm::vec3 camera_right_vector; // We will have up, right and front of the camera

  glm::vec3 camera_look_at; // I don't know if we should keep it here
} Camera;

void calculate_camera_front(Camera* camera);


#endif // _H_CAMERA

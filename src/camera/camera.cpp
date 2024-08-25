#include <camera.hpp>

#include <stdio.h>

void calculate_camera_front(Camera* camera) {
  // @NOT_IMPLEMENTED
  printf("Camera: %p\n", camera);

  // Having camera pos and look_at we can calculate camera_front
  camera->camera_front = glm::normalize(camera->camera_look_at - camera->camera_pos); // Does normalize give us unit vector?
  camera->camera_right_vector = glm::normalize(-glm::cross(camera->camera_front, camera->camera_up_vector));
  camera->camera_up_vector = glm::normalize(-glm::cross(camera->camera_front, camera->camera_right_vector));

  printf("Camera look at: %f %f %f\n", camera->camera_look_at.x, camera->camera_look_at.y, camera->camera_look_at.z);
  printf("Camera pos: %f %f %f\n", camera->camera_pos.x, camera->camera_pos.y, camera->camera_pos.z);

  printf("\n");

  printf("Camera front: %f %f %f\n", camera->camera_front.x, camera->camera_front.y, camera->camera_front.z);
  printf("Camera up vector: %f %f %f\n", camera->camera_up_vector.x, camera->camera_up_vector.y, camera->camera_up_vector.z);
  printf("Camera right vector: %f %f %f\n", camera->camera_right_vector.x, camera->camera_right_vector.y, camera->camera_right_vector.z);
  printf("\n");

}


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


#endif // _H_CAMERA

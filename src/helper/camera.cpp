// Helper Camera Utility
#include "../../inc/helper/camera.h"

GLfloat cameraEyeX;
GLfloat cameraEyeY;
GLfloat cameraEyeZ;

GLfloat cameraCenterX;
GLfloat cameraCenterY;
GLfloat cameraCenterZ;

GLfloat cameraUpX;
GLfloat cameraUpY;
GLfloat cameraUpZ;

vmath::mat4 viewMatrix;

void setCamera(void)
{
	Camera camera;
	camera.eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
	camera.center = { cameraCenterX, cameraCenterY, cameraCenterZ };
	camera.up = { cameraUpX, cameraUpY, cameraUpZ };

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
}

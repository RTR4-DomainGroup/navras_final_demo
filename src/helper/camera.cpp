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

Camera camera;

void setCamera(void)
{
	camera.eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
	camera.center = { cameraCenterX, cameraCenterY, cameraCenterZ };
	camera.up = { cameraUpX, cameraUpY, cameraUpZ };
}

GLfloat impreciselerp(GLfloat v0, GLfloat v1, GLfloat t)
{
	return v0 + t * (v1 - v0);
}

GLfloat preciselerp(GLfloat v0, GLfloat v1, GLfloat t)
{
	return (1 - t) * v0 + t * v1;
}

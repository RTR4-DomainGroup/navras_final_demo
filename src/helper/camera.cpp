// Helper Camera Utility
#include <stdlib.h>
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

// camera related variables for movement in scene during debugging
float cameraCounterSideWays = 3.2f;
float cameraCounterUpDownWays = 3.2f;

Camera camera;

void initializeCamera(Camera* camera)
{
	camera = (Camera*)malloc(sizeof(Camera));
}

void setCamera(void)
{
	camera.eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
	camera.center = { cameraCenterX, cameraCenterY, cameraCenterZ };
	camera.up = { cameraUpX, cameraUpY, cameraUpZ };
}


void resetCamera(void)
{
	cameraEyeX = 0.0f;
	cameraEyeY = 0.0f;
	cameraEyeZ = 6.0f;

	cameraCenterX = 0.0f;
	cameraCenterY = 0.0f;
	cameraCenterZ = 0.0f;

	cameraUpX = 0.0f;
	cameraUpY = 1.0f;
	cameraUpZ = 0.0f;

	cameraCounterSideWays = 3.2f;
	cameraCounterUpDownWays = 3.2f;
}

//void setCamera(Camera *camera)
//{
//	camera->eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
//	camera->center = { cameraCenterX, cameraCenterY, cameraCenterZ };
//	camera->up = { cameraUpX, cameraUpY, cameraUpZ };
//}

GLfloat impreciselerp(GLfloat v0, GLfloat v1, GLfloat t)
{
	return v0 + t * (v1 - v0);
}

GLfloat preciselerp(GLfloat v0, GLfloat v1, GLfloat t)
{
	return (1 - t) * v0 + t * v1;
}

void rotateCamera(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat radius, GLfloat angle)
{
	// code
	float angleRadian = angle * M_PI / 180.0f;
	cameraEyeX = centerX + (radius * cos(angleRadian));
	cameraEyeY = centerY;
	cameraEyeZ = centerZ + (radius * sin(angleRadian));

	cameraCenterX = centerX;
	cameraCenterY = centerY;
	cameraCenterZ = centerZ;

	camera.eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
	camera.center = { cameraCenterX, cameraCenterY, cameraCenterZ };
	camera.up = { cameraUpX, cameraUpY, cameraUpZ };
}

//void rotateCamera(Camera *camera, GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat radius, GLfloat angle)
//{
//	// code
//	float angleRadian = angle * M_PI / 180.0f;
//	cameraEyeX = centerX + (radius * cos(angleRadian));
//	cameraEyeY = centerY;
//	cameraEyeZ = centerZ + (radius * sin(angleRadian));
//	
//	cameraCenterX = centerX;
//	cameraCenterY = centerY;
//	cameraCenterZ = centerZ;
//
//	camera->eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
//	camera->center = { cameraCenterX, cameraCenterY, cameraCenterZ };
//	camera->up = { cameraUpX, cameraUpY, cameraUpZ };
//}

//void uninitializeCamera(Camera *camera)
//{
//	if (camera != NULL)
//	{
//		free(camera);
//		camera = NULL;
//	}
//}

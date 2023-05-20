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

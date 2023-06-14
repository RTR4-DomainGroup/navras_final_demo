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


// initial camera
static Camera iCamera;


// camera related variables for movement in scene during debugging
float cameraCounterSideWays = 3.2f;
float cameraCounterUpDownWays = 3.2f;

Camera camera;

void initializeCamera(Camera* camera)
{
	camera = (Camera*)malloc(sizeof(Camera));
}

void setCamera(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
	cameraEyeX = eyeX;
	cameraEyeY = eyeY;
	cameraEyeZ = eyeZ;

	cameraCenterX = centerX;
	cameraCenterY = centerY;
	cameraCenterZ = centerZ;

	cameraUpX = upX;
	cameraUpY = upY;
	cameraUpZ = upZ;

	iCamera = {{ cameraEyeX, cameraEyeY, cameraEyeZ }, { cameraCenterX, cameraCenterY, cameraCenterZ }, { cameraUpX, cameraUpY, cameraUpZ }};
}

void displayCamera(void)
{
	camera.eye = { cameraEyeX, cameraEyeY, cameraEyeZ };
	camera.center = { cameraCenterX, cameraCenterY, cameraCenterZ };
	camera.up = { cameraUpX, cameraUpY, cameraUpZ };
}


void resetCamera(void)
{
	cameraEyeX = iCamera.eye[0];
	cameraEyeY = iCamera.eye[1];
	cameraEyeZ = iCamera.eye[2];

	cameraCenterX = iCamera.center[0];
	cameraCenterY = iCamera.center[1];
	cameraCenterZ = iCamera.center[2];

	cameraUpX = iCamera.up[0];
	cameraUpY = iCamera.up[1];
	cameraUpZ = iCamera.up[2];

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

void preciselerp_lookat(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
		cameraEyeX = preciselerp(cameraEyeX, eyeX, 0.002f);
		cameraEyeY = preciselerp(cameraEyeY, eyeY, 0.002f);
		cameraEyeZ = preciselerp(cameraEyeZ, eyeZ, 0.002f);

		cameraCenterX = preciselerp(cameraCenterX, centerX, 0.002f);
		cameraCenterY = preciselerp(cameraCenterY, centerY, 0.002f);
		cameraCenterZ = preciselerp(cameraCenterZ, centerZ, 0.002f);

		cameraUpX = upX;
		cameraUpY = upY;
		cameraUpZ = upZ;
}

void rotateCamera(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat radius, GLfloat angle, bool centerYConfigured)
{
	// code
	float angleRadian = angle * M_PI / 180.0f;
	cameraEyeX = centerX + (radius * cos(angleRadian));
	if (centerYConfigured == true)
		cameraEyeY = centerY;
		
	cameraEyeZ = centerZ + (radius * sin(angleRadian));

	cameraCenterX = centerX;
	if (centerYConfigured == true)
		cameraCenterY = cameraEyeY;
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

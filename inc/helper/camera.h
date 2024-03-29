#pragma once

// Open GL headers
#include <GL/glew.h>
#include <GL/gl.h>

#include "vmath.h"

struct Camera
{
	vmath::vec3 eye;
	vmath::vec3 center;
	vmath::vec3 up;
};

extern GLfloat cameraEyeX;
extern GLfloat cameraEyeY;
extern GLfloat cameraEyeZ;
 
extern GLfloat cameraCenterX;
extern GLfloat cameraCenterY;
extern GLfloat cameraCenterZ;
 
extern GLfloat cameraUpX;
extern GLfloat cameraUpY;
extern GLfloat cameraUpZ;

extern Camera camera;

//void initializeCamera(Camera*);
void setCamera(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

void displayCamera(void);
//void setCamera(Camera*);

GLfloat impreciselerp(GLfloat, GLfloat, GLfloat);
GLfloat preciselerp(GLfloat, GLfloat, GLfloat);

void rotateCamera(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void resetCamera(void);


//void rotateCamera(Camera, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

//void uninitializeCamera(Camera*);

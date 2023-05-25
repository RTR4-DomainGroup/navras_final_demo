
// internal headers
#include "../../inc/helper/common.h"
#include "../../inc/helper/geometrytypes.h"
#include "../../inc/helper/camera.h"
#include "../../inc/debug/debug_transformation.h"

// transformation controllers
TRANFORM tf_t; // translation
TRANFORM tf_s; // scale
TRANFORM tf_r; // rotation
GLfloat tf_Speed = 0.05f; // transformation speed


static bool translateMode = false;
static bool scaleMode = false;
static bool rotateMode = false;
static bool cameraMode = false;

// extern these variables in your WndProc 
extern GLbyte charPressed;
extern GLuint keyPressed;

// extern
// camera related variables for movement in scene during debugging
extern float cameraCounterSideWays;
extern float cameraCounterUpDownWays;

void debug_tranformation(void)
{
	// local trnasformation
	static TRANFORM l_tsl; // local translation
	static TRANFORM l_scl; // local scale
	static TRANFORM l_rot; // local rotation
	static GLfloat ltf_R, ltf_Speed;
	static GLboolean firstCall = GL_TRUE;
	if(firstCall)
	{
		l_tsl = tf_t;  
		l_scl = tf_s;  
		l_rot = tf_r; 
		firstCall = GL_FALSE;
	}

	// code
	if(charPressed)
	{
		GLubyte charHandled = GL_TRUE;

		switch(charPressed)
		{
		case 't':
			translateMode = translateMode ? false : true;
			break;
		case 's':
			if(cameraMode)
			{
				cameraEyeZ = cameraEyeZ + 0.25f;
				cameraCenterZ = cameraCenterZ + 0.25f;
			}
			else {
				scaleMode = scaleMode ? false : true;
			}
			break;
		case 'r':
			if(cameraMode)
				resetCamera();
			else	
				rotateMode = rotateMode ? false : true;
			break;
		case 'x':
		case 'X':
			if(translateMode) {
				if('x' == charPressed)
					tf_t.x += tf_Speed;
				else
					tf_t.x -= tf_Speed;
				LOG("X translation changed to %.02ff\n", tf_t.x);
			}
			if(scaleMode) {
				if('x' == charPressed)
					tf_s.x += tf_Speed;
				else
					tf_s.x -= tf_Speed;
				LOG("X scale changed to %.02ff\n", tf_s.x);
			}
			if(rotateMode) {
				if('x' == charPressed)
					tf_r.x += tf_Speed;
				else
					tf_r.x -= tf_Speed;
				LOG("X rotation changed to %.02ff\n", tf_r.x);
			}
			break;
		case 'y':
		case 'Y':
			if(translateMode) {
				if('y' == charPressed)
					tf_t.y += tf_Speed;
				else
					tf_t.y -= tf_Speed;
				LOG("Y translation changed to %.02ff\n", tf_t.y);
			}
			if(scaleMode) {
				if('y' == charPressed)
					tf_s.y += tf_Speed;
				else
					tf_s.y -= tf_Speed;
				LOG("Y transform changed to %.02ff\n", tf_s.y);
			}
			if(rotateMode) {
				if('y' == charPressed)
					tf_r.y = 1.0;
				else
					tf_r.y = 0.0f;
				LOG("Y rotation changed to %.02ff\n", tf_r.y);
			}
			break;
		case 'z':
		case 'Z':
			if(translateMode) {
				if('z' == charPressed)
					tf_t.z += tf_Speed;
				else
					tf_t.z -= tf_Speed;
				LOG("Z translation changed to %.02ff\n", tf_t.z);
			}
			if(scaleMode) {
				if('z' == charPressed)
					tf_s.z += tf_Speed;
				else
					tf_s.z -= tf_Speed;
				LOG("Z scale changed to %.02ff\n", tf_s.z);
			}
			if(rotateMode) {
				if('z' == charPressed)
					tf_r.z = 1.0;
				else
					tf_r.z = 0.0f;
				LOG("Z rotation changed to %.02ff\n", tf_r.z);
			}
			break;
		case 'p':
			LOG("\n");
			LOG("lookAt([%f, %f, %f], [%f, %f, %f] [%f, %f, %f])\n", 
				cameraEyeX, cameraEyeY, cameraEyeZ, 
				cameraCenterX, cameraCenterY, cameraCenterZ, 
				cameraUpX, cameraUpY, cameraUpZ);
			LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			LOG("Rotation is (%.02ff) %.02ff, %.02ff, %.02ff\n", tf_r.x, tf_r.y, tf_r.z);
			break;
		case '+':
			tf_Speed += 0.04f;
			if(tf_Speed < 0)
				tf_Speed = 0.0f;
			break;
		case '-':
			tf_Speed -= 0.02f;
			if(tf_Speed < 0)
				tf_Speed = 0.0f;
			break;
		// case 'c':
		// 	tf_t.x = tf_t.y = tf_t.z = tf_R = tf_Speed = 0.0f;
		// 	break;
		case 'C':
			tf_t = l_tsl;  
			tf_s = l_scl;  
			tf_r = l_rot; 
			break;
		case 'c':
			cameraMode = cameraMode ? false : true; 
			break;
		case 'W':
		case 'w':
			if(cameraMode)
			{
				cameraEyeZ = cameraEyeZ - 0.25f;
				cameraCenterZ = cameraCenterZ - 0.25f;
			}
			break;

		case 'A':
		case 'a':
			if(cameraMode)
			{
				cameraEyeX = cameraEyeX - 0.25f;
				cameraCenterX = cameraCenterX - 0.25f;
			}
			break;
		case 'D':
		case 'd':
			if(cameraMode)
			{
				cameraEyeX = cameraEyeX + 0.25f;
				cameraCenterX = cameraCenterX + 0.25f;
			}
			break;
		case 'Q':
		case 'q':
			if(cameraMode)
			{
				cameraEyeY = cameraEyeY - 0.25f;
				cameraCenterY = cameraCenterY - 0.25f;
			}
			break;
		case 'E':
		case 'e':
			if(cameraMode)
			{
				cameraEyeY = cameraEyeY + 0.25f;
				cameraCenterY = cameraCenterY + 0.25f;
			}
			break;
		default:
			charHandled = GL_FALSE;
			break;
		}

		if(GL_TRUE == charHandled)
		{
			if(ltf_Speed != tf_Speed)
			{
				ltf_Speed = tf_Speed;
				LOG("TF speed changed to %.02ff\n", ltf_Speed);
			}
		}
		charPressed = 0;
	}

	if(keyPressed)
	{
		switch (keyPressed)
		{
		case VK_UP:	// Up
			cameraCenterY = sin(cameraCounterUpDownWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterUpDownWays) * 360.0f;
			cameraCounterUpDownWays += 0.025f;
			break;
		case VK_DOWN:	// down
			cameraCenterY = sin(cameraCounterUpDownWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterUpDownWays) * 360.0f;
			cameraCounterUpDownWays -= 0.025f;
			break;
		case VK_LEFT:	// left
			//LOG("cameraCounterSideWays : %f\n", cameraCounterSideWays);
			cameraCenterX = sin(cameraCounterSideWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterSideWays) * 360.0f;
			cameraCounterSideWays += 0.025f;
			break;
		case VK_RIGHT:	// right
			cameraCenterX = sin(cameraCounterSideWays) * 360.0f;
			cameraCenterZ = cos(cameraCounterSideWays) * 360.0f;
			cameraCounterSideWays -= 0.025f;
			break;
		
		default:
			break;
		}
		keyPressed = 0;
	}
}


void update_transformations(vmath::mat4& translationMatrix, vmath::mat4& scaleMatrix, vmath::mat4& rotationMatrix) 
{
	static bool firstCall = 1;

	// external debugging varaible
	if(firstCall) {
		tf_t = {translationMatrix[0][0], translationMatrix[1][1], translationMatrix[2][2]}; // tree pos
		tf_s = {scaleMatrix[0][0], scaleMatrix[1][1], scaleMatrix[2][2]}; // tree scale
		tf_r = {rotationMatrix[0][0], rotationMatrix[1][1], rotationMatrix[2][2]}; // tree rotate
		tf_Speed = 0.05f;
		firstCall = 0;
	}

	translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	// scaleMatrix = vmath::scale(tf_s.x, tf_s.y, tf_s.z);
	scaleMatrix = vmath::scale(tf_s.x, tf_s.x, tf_s.x);
	mat4 rotationMatrix_x = vmath::rotate(tf_r.x, 1.0f, 0.0f, 0.0f);
	mat4 rotationMatrix_y = vmath::rotate(tf_r.y, 0.0f, 1.0f, 0.0f);
	mat4 rotationMatrix_z = vmath::rotate(tf_r.z, 1.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
}



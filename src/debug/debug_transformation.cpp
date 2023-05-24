
// internal headers
#include "../../inc/helper/common.h"
#include "../../inc/helper/geometrytypes.h"

// transformation controllers
TRANFORM tf_t; // translation
TRANFORM tf_s; // scale
TRANFORM tf_r; // rotation
GLfloat rAngle;
GLfloat tf_Speed = 0.05f; // transformation speed

static bool translateMode = false;
static bool scaleMode = false;
static bool rotateMode = false;

// extern these variables in your WndProc 
extern GLbyte charPressed;
extern GLuint keyPressed;

void debug_tranformation(void)
{
	// local trnasformation
	static TRANFORM ltf;
	static GLfloat ltf_R, ltf_Speed;
	static GLboolean firstCall = GL_TRUE;
	if(firstCall)
	{
		ltf.x = tf_t.x;  
		ltf.y = tf_t.y;  
		ltf.z = tf_t.z; 
		ltf_R = rAngle;
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
			scaleMode = scaleMode ? false : true;
			break;
		case 'r':
			rotateMode = rotateMode ? false : true;
			break;
		case 'x':
		case 'X':
			if(translateMode) {
				if('x' == charPressed)
					tf_t.x += tf_Speed;
				else
					tf_t.x -= tf_Speed;
				// LOG("X translation changed to %.02ff\n", tf_t.x);
			}
			if(scaleMode) {
				if('x' == charPressed)
					tf_s.x += tf_Speed;
				else
					tf_s.x -= tf_Speed;
				// LOG("X scale changed to %.02ff\n", tf_s.x);
			}
			if(rotateMode) {
				if('x' == charPressed)
					tf_r.x = 1.0;
				else
					tf_r.x = 0.0f;
				// LOG("X rotation changed to %.02ff\n", tf_r.x);
			}
			break;
		case 'y':
		case 'Y':
			if(translateMode) {
				if('y' == charPressed)
					tf_t.y += tf_Speed;
				else
					tf_t.y -= tf_Speed;
				// LOG("Y translation changed to %.02ff\n", tf_t.y);
			}
			if(scaleMode) {
				if('y' == charPressed)
					tf_s.y += tf_Speed;
				else
					tf_s.y -= tf_Speed;
				// LOG("Y transform changed to %.02ff\n", tf_s.y);
			}
			if(rotateMode) {
				if('y' == charPressed)
					tf_r.y = 1.0;
				else
					tf_r.y = 0.0f;
				// LOG("Y rotation changed to %.02ff\n", tf_r.y);
			}
			break;
		case 'z':
		case 'Z':
			if(translateMode) {
				if('z' == charPressed)
					tf_t.z += tf_Speed;
				else
					tf_t.z -= tf_Speed;
				// LOG("Z translation changed to %.02ff\n", tf_t.z);
			}
			if(scaleMode) {
				if('z' == charPressed)
					tf_s.z += tf_Speed;
				else
					tf_s.z -= tf_Speed;
				// LOG("Z scale changed to %.02ff\n", tf_s.z);
			}
			if(rotateMode) {
				if('z' == charPressed)
					tf_r.z = 1.0;
				else
					tf_r.z = 0.0f;
				// LOG("Z rotation changed to %.02ff\n", tf_r.z);
			}
			break;
		case 'p':
			LOG("\n");
			LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			LOG("Rotation is (%.02ff) %.02ff, %.02ff, %.02ff\n", rAngle, tf_r.x, tf_r.y, tf_r.z);
			break;
		case 'g':
			if(rotateMode) {
				rAngle += tf_Speed;
				// LOG("R transform changed to %.02ff\n", rAngle);
			}
			break;
		case 'G':
			if(rotateMode) {
				rAngle -= tf_Speed;
				// LOG("R transform changed to %.02ff\n", rAngle);
			}
			break;
		case '+':
			tf_Speed += 0.05f;
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
			tf_t.x = ltf.x;
			tf_t.y = ltf.y;
			tf_t.z = ltf.z;
			rAngle = ltf_R;
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
				// LOG("TF speed changed to %.02ff\n", ltf_Speed);
			}
		}
		charPressed = 0;
	}

	if(keyPressed)
	{
		keyPressed = 0;
	}
}

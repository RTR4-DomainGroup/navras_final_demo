
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
int tf_Object = 0; // transformation object


static bool translateMode = false;
static bool scaleMode = false;
static bool rotateMode = false;
static bool cameraMode = false;

static bool updateFirstCall = true;
static bool debugFirstCall = true;

// extern these variables in your WndProc 
extern GLbyte charPressed;
extern GLuint keyPressed;

extern GLuint numPressed;

// extern
// camera related variables for movement in scene during debugging
extern float cameraCounterSideWays;
extern float cameraCounterUpDownWays;

void debug_tranformation(void)
{
	// local trnasformation
	static TRANFORM ltf_t; // local translation
	static TRANFORM ltf_s; // local scale
	static TRANFORM ltf_r; // local rotation
	static GLfloat ltf_Speed;
	if(debugFirstCall)
	{
		ltf_t = tf_t;  
		ltf_s = tf_s;  
		ltf_r = tf_r; 
		ltf_Speed = tf_Speed;
		debugFirstCall = false;
	}

	// code
	if(charPressed)
	{
		GLubyte charHandled = GL_TRUE;

		switch(charPressed)
		{
		case 't': // translate
			translateMode = scaleMode = rotateMode = cameraMode = false;
			translateMode = translateMode ? false : true;
			LOG("switched to translation mode: %d\n", translateMode);
			break;
		case 's': // scale or cameraZ 
			if(cameraMode)
			{
				cameraEyeZ = cameraEyeZ + tf_Speed;
				cameraCenterZ = cameraCenterZ + tf_Speed;
			}
			else {
				translateMode = scaleMode = rotateMode = cameraMode = false;
				scaleMode = scaleMode ? false : true;
				LOG("switched to scale mode: %d\n", scaleMode);
			}
			break;
		case 'r': // rotate or camera reset
			if(cameraMode)
				resetCamera();
			else	{
				translateMode = scaleMode = rotateMode = cameraMode = false;
				rotateMode = rotateMode ? false : true;
				LOG("switched to rotation mode: %d\n", rotateMode);
			}
			break;
		case 'x':
		case 'X':
			if(translateMode) {
				if('x' == charPressed)
					tf_t.x += tf_Speed;
				else
					tf_t.x -= tf_Speed;
				LOG("X translation changed to %.02ff\n", tf_t.x);
				LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			}
			if(scaleMode) {
				if('x' == charPressed)
					tf_s.x += tf_Speed;
				else
					tf_s.x -= tf_Speed;
				LOG("X scale changed to %.02ff\n", tf_s.x);
				LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			}
			if(rotateMode) {
				if('x' == charPressed)
					tf_r.x += tf_Speed;
				else
					tf_r.x -= tf_Speed;
				LOG("X rotation changed to %.02ff\n", tf_r.x);
				LOG("Rotation is %.02ff, %.02ff, %.02ff\n", tf_r.x, tf_r.y, tf_r.z);
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
				LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			}
			if(scaleMode) {
				if('y' == charPressed)
					tf_s.y += tf_Speed;
				else
					tf_s.y -= tf_Speed;
				LOG("Y transform changed to %.02ff\n", tf_s.y);
				LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			}
			if(rotateMode) {
				if('y' == charPressed)
					tf_r.y += tf_Speed;
				else
					tf_r.y -= tf_Speed;
				LOG("Y rotation changed to %.02ff\n", tf_r.y);
				LOG("Rotation is %.02ff, %.02ff, %.02ff\n", tf_r.x, tf_r.y, tf_r.z);
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
				LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			}
			if(scaleMode) {
				if('z' == charPressed)
					tf_s.z += tf_Speed;
				else
					tf_s.z -= tf_Speed;
				LOG("Z scale changed to %.02ff\n", tf_s.z);
				LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			}
			if(rotateMode) {
				if('z' == charPressed)
					tf_r.z += tf_Speed;
				else
					tf_r.z -= tf_Speed;
				LOG("Z rotation changed to %.02ff\n", tf_r.z);
				LOG("Rotation is %.02ff, %.02ff, %.02ff\n", tf_r.x, tf_r.y, tf_r.z);
			}
			break;
		case 'p':
			LOG("\n");
			LOG("lookAt(%.02ff, %.02ff, %.02ff, %.02ff, %.02ff, %.02ff, %.02ff, %.02ff, %.02ff)\n", 
				cameraEyeX, cameraEyeY, cameraEyeZ, 
				cameraCenterX, cameraCenterY, cameraCenterZ, 
				cameraUpX, cameraUpY, cameraUpZ);
			LOG("Translation is %.02ff, %.02ff, %.02ff\n", tf_t.x, tf_t.y, tf_t.z);
			LOG("Scale is %.02ff, %.02ff, %.02ff\n", tf_s.x, tf_s.y, tf_s.z);
			LOG("Rotation is %.02ff, %.02ff, %.02ff\n", tf_r.x, tf_r.y, tf_r.z);
			break;
		case '+':
			tf_Speed += 0.02f;
			LOG("TF speed changed to %.02ff\n", tf_Speed);
			break;
		case '-':
			tf_Speed -= 0.01f;
			if(tf_Speed < 0)
				tf_Speed = 0.0f;
			LOG("TF speed changed to %.02ff\n", tf_Speed);
			break;
		// case 'c':
		// 	tf_t.x = tf_t.y = tf_t.z = tf_R = tf_Speed = 0.0f;
		// 	break;
		case 'T':
			tf_t = ltf_t;  
			tf_s = ltf_s;  
			tf_r = ltf_r; 
			tf_Speed = ltf_Speed;
			tf_Object = 0;
			break;
		case 'c':
			LOG("switched to camera mode\n");
			// translateMode = scaleMode = rotateMode = cameraMode = false;
			cameraMode = cameraMode ? false : true; 
			break;
		case 'W':
		case 'w':
			if(cameraMode)
			{
				cameraEyeZ = cameraEyeZ - tf_Speed;
				cameraCenterZ = cameraCenterZ - tf_Speed;
			}
			break;

		case 'A':
		case 'a':
			if(cameraMode)
			{
				cameraEyeX = cameraEyeX - tf_Speed;
				cameraCenterX = cameraCenterX - tf_Speed;
			}
			break;
		case 'D':
		case 'd':
			if(cameraMode)
			{
				cameraEyeX = cameraEyeX + tf_Speed;
				cameraCenterX = cameraCenterX + tf_Speed;
			}
			break;
		case 'Q':
		case 'q':
			if(cameraMode)
			{
				cameraEyeY = cameraEyeY - tf_Speed;
				cameraCenterY = cameraCenterY - tf_Speed;
			}
			break;
		case 'E':
		case 'e':
			if(cameraMode)
			{
				cameraEyeY = cameraEyeY + tf_Speed;
				cameraCenterY = cameraCenterY + tf_Speed;
			}
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			LOG("Num key pressed %c \n", charPressed, charPressed);
			updateFirstCall = true;
			debugFirstCall = true;
			tf_t = {0.0f, 0.0f, 0.0f};
			tf_r = {0.0f, 0.0f, 0.0f};
			tf_s = {0.0f, 0.0f, 0.0f};
			tf_Object = charPressed - '0';
			LOG("Object selected %d \n", tf_Object);
			break;
		default:
			charHandled = GL_FALSE;
			break;
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


void update_transformations(vmath::mat4* translationMatrix, vmath::mat4* scaleMatrix, vmath::mat4* rotationMatrix, TRANFORM* vector) 
{

	// external debugging varaible
	static bool perAxisDiffScale = false;

	if(updateFirstCall) {
		// update tf_* variables only at first calls, later those will update based on events 
		if(translationMatrix)
			tf_t = {(*translationMatrix)[3][0], (*translationMatrix)[3][1], (*translationMatrix)[3][2]}; // pos
		if(scaleMatrix) {
			tf_s = {(*scaleMatrix)[0][0], (*scaleMatrix)[1][1], (*scaleMatrix)[2][2]}; // tree scale
			if(tf_s.x != tf_s.y || tf_s.x != tf_s.z || tf_s.y != tf_s.z)
				perAxisDiffScale = true;
			else 
				perAxisDiffScale = false;
		}
		if(vector)
			tf_r = {vector->x, vector->y, vector->z}; // tree rotate
		else
			tf_r = {0.0f, 0.0f, 0.0f};
		tf_Speed = 0.25f;
		updateFirstCall = false;
	}

	if(translationMatrix)
		*translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	if(scaleMatrix) {
		if (perAxisDiffScale)
			*scaleMatrix = vmath::scale(tf_s.x, tf_s.y, tf_s.z);
		else
			*scaleMatrix = vmath::scale(tf_s.x, tf_s.x, tf_s.x);
	}
	mat4 rotationMatrix_x = vmath::rotate(tf_r.x, 1.0f, 0.0f, 0.0f);
	mat4 rotationMatrix_y = vmath::rotate(tf_r.y, 0.0f, 1.0f, 0.0f);
	mat4 rotationMatrix_z = vmath::rotate(tf_r.z, 1.0f, 0.0f, 1.0f);
	if(rotationMatrix)
		*rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	if(vector)
		*vector = tf_r;
}


void update_transformations_glm(glm::mat4* translationMatrix, glm::mat4* scaleMatrix, glm::mat4* rotationMatrix, TRANFORM* vector)
{
	// external debugging varaible
	static bool perAxisDiffScale = false;
	if(updateFirstCall) {
		if (translationMatrix)
			tf_t = { (*translationMatrix)[3][0], (*translationMatrix)[3][1], (*translationMatrix)[3][2] }; // pos
		if (scaleMatrix) {
			tf_s = { (*scaleMatrix)[0][0], (*scaleMatrix)[1][1], (*scaleMatrix)[2][2] }; // tree scale
			if(tf_s.x != tf_s.y || tf_s.x != tf_s.z || tf_s.y != tf_s.z)
			{
				perAxisDiffScale = true;
			}
		}
		if (vector)
			tf_r = { vector->x, vector->y, vector->z }; // tree rotate
		else
			tf_r = { 0.0f, 0.0f, 0.0f };
		tf_Speed = 0.25f;
		updateFirstCall = false;
	}

	if (translationMatrix)
		*translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tf_t.x, tf_t.y, tf_t.z));
	if (scaleMatrix)

	// if (perAxisDiffScale)
	// 	*scaleMatrix = glm::scale(tf_s.x, tf_s.y, tf_s.z);
	// else
		*scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(tf_s.x, tf_s.x, tf_s.x));
	
	glm::mat4 rotationMatrix_x = glm::rotate(glm::mat4(1.0f), tf_r.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationMatrix_y = glm::rotate(glm::mat4(1.0f), tf_r.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrix_z = glm::rotate(glm::mat4(1.0f), tf_r.z, glm::vec3(0.0f, 0.0f, 1.0f));
	if (rotationMatrix)
		*rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
}

void print_vector(const vmath::vec4& vector) {
	LOG(""
	"%.02ff %.02ff %.02ff %.02ff \n\n", 
	vector[0], vector[1], vector[2], vector[3]);
}

void print_matrix(const vmath::mat4& matrix) {
	LOG(""
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n\n", 
	matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
	matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
	matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
	matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
}

void print_matrix_glm(const glm::mat4& matrix) {
	LOG(""
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n"
	"%.02ff %.02ff %.02ff %.02ff \n\n", 
	matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
	matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
	matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
	matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
}


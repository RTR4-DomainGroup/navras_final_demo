#include "../../inc/helper/common.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/texture_loader.h"

#include "../../inc/shaders/BillboardingShader.h"

// // vertex array object
// GLuint vao_instancing;
// GLuint vao_basequad;

// // vertex buffer object
// GLuint vbo_position; 
// GLuint vbo_position_basequad; 
// GLuint vbo_texcoords; 

extern mat4 viewMatrix;
extern mat4 perspectiveProjectionMatrix;


int initializeBillboarding(void)
{
	// local variable
	int retVal = 0;

    // Code
    // declaration of vertex data arrays
    // In PP: glVertex3f, glTexCords, etx will be replaced by arrays

    GLfloat instance_positions[NO_OF_INSTANCES * 4] = {};
    // generate positions per instance
    for(int i = 0; i < NO_OF_INSTANCES; i++)
    {
		instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (X_MAX - X_MIN)) + X_MIN;
		instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (Y_MAX - Y_MIN)) + Y_MIN;
		instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (Z_MAX - Z_MIN)) + Z_MIN;
		instance_positions[(i*4)+3] = 1.0f;
		LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
    }

    initializeInstancedQuad(NO_OF_INSTANCES, instance_positions);
    // initializeQuad();


	return (retVal);
}

// void displayBillboarding(void)
// {
//     BillboardingUniform billboardingUniform = useBillboardingShader();

//     // Code

//     // transformations
//     mat4 translationMatrix = mat4::identity();
//     mat4 rotationMatrix = mat4::identity();
//     mat4 scaleMatrix = mat4::identity();

//     mat4 modelMatrix = mat4::identity();

// //////////////////////////////
//     // base quad

//     // repeat the recoded casset again
//     translationMatrix = vmath::translate(0.0f, -6.0f, 0.0f);
//     rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
//     // scaleMatrix = vmath::scale(40.0f, 1.0f, 40.0f);

//     modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

//     // send to shader
//     glUniformMatrix4fv(
//         billboardingUniform.modelMatrixUniform, // which uniform
//         1, // 
//         GL_FALSE,
//         modelMatrix
//     );

//     glUniformMatrix4fv(
//         billboardingUniform.viewMatrixUniform, // which uniform
//         1, // 
//         GL_FALSE,
//         viewMatrix
//     );

//     glUniformMatrix4fv(
//         billboardingUniform.projectionMatrixUniform, // which uniform4
//         1, // 
//         GL_FALSE,
//         perspectiveProjectionMatrix
//     );
//     glUniform1i(billboardingUniform.billboardingEnableUniform, 0);

//     displayQuad();

// //////////////////////////////////////////
//     // instanced quads with grass texture

//     translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
//     if(texture_grass.height > texture_grass.width)
//         scaleMatrix = vmath::scale(texture_grass.width/(GLfloat)texture_grass.height, 1.0f, 1.0f);
//     else
//         scaleMatrix = vmath::scale(1.0f, texture_grass.height/(GLfloat)texture_grass.width, 1.0f);
//     modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

//     // send to shader
//     glUniformMatrix4fv(
//         billboardingUniform.modelMatrixUniform, // which uniform
//         1, // 
//         GL_FALSE,
//         modelMatrix
//     );
//     glUniformMatrix4fv(
//         billboardingUniform.viewMatrixUniform, // which uniform
//         1, // 
//         GL_FALSE,
//         viewMatrix
//     );
//     glUniformMatrix4fv(
//         billboardingUniform.projectionMatrixUniform, // which uniform
//         1, // 
//         GL_FALSE,
//         perspectiveProjectionMatrix
//     );

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture_grass.id);
//     glUniform1i(billboardingUniform.textureSamplerUniform, 0);

//     // if(bBillboardingEnabled)
//     glUniform1i(billboardingUniform.billboardingEnableUniform, 1);
//     // else
//     //     glUniform1i(billboardingUniform.billboardingEnableUniform, 0);

//     displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
    
//     glBindTexture(GL_TEXTURE_2D, 0);
// }


void displayBillboardingGrass(void)
{
    glActiveTexture(GL_TEXTURE0);

    displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

}

void displayBillboardingFlower(void)
{
    glActiveTexture(GL_TEXTURE0);

    displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

}



float angleForYRotation = 0.0f;
float radius = 30.0f;

void updateBillboarding(void)
{
    angleForYRotation = angleForYRotation + 0.1f;
    if(angleForYRotation >= 360.0)
        angleForYRotation = 0.0;

    // float xDistance = 0.0f;
    // static float zDistance = 0.0f;
    // zDistance = tf_R;

    // float radn = DEG2RADN(angleForYRotation);
    // camera.Eye[0] = xDistance + (radius * cos(radn));
    // camera.Eye[1] = 0;
    // camera.Eye[2] = zDistance + (radius * sin(radn));
}


void uninitializeBillboarding(void)
{
    // Code
    
    uninitializeQuad();
    uninitializeInstancedQuads();
}


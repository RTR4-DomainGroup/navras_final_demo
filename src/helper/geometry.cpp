#include "../../inc/common.h"
#include "../../inc/geometry.h"

GLuint vao_Cube;
GLuint vbo_Cube;

GLuint vao_CloudQuad;
GLuint vbo_CloudQuadPosition;
GLuint vbo_CloudQuadNormal;

int initializeGeometry(void)
{

    // Code
    //const GLfloat cubePCNT[] = {
    //                                                //PCNT
    //        // positions                 //colors               //normals                   //texture

    //        // Top face               // Top face                 // Top face              // Top face
    //        1.0f, 1.0f, -1.0f,       0.0f,0.0f,1.0f,          0.0f, 1.0f, 0.0f,            1.0f,1.0f,
    //        -1.0f, 1.0f, -1.0f,      0.0f,0.0f,1.0f,          0.0f, 1.0f, 0.0f,            0.0f,1.0f,
    //        -1.0f, 1.0f, 1.0f,       0.0f,0.0f,1.0f,          0.0f, 1.0f, 0.0f,            0.0f,0.0f,
    //        1.0f, 1.0f, 1.0f,        0.0f,0.0f,1.0f,          0.0f, 1.0f, 0.0f,            1.0f,0.0f,

    //        // Bottom face            // Bottom face             // Bottom face            // Bottom face
    //        1.0f, -1.0f, -1.0f,      0.0f,1.0f,0.0f,         0.0f, -1.0f, 0.0f,            1.0f,1.0f,
    //        -1.0f, -1.0f, -1.0f,     0.0f,1.0f,0.0f,         0.0f, -1.0f, 0.0f,            0.0f,1.0f,
    //        -1.0f, -1.0f, 1.0f,      0.0f,1.0f,0.0f,         0.0f, -1.0f, 0.0f,            0.0f,0.0f,
    //        1.0f, -1.0f, 1.0f,       0.0f,1.0f,0.0f,         0.0f, -1.0f, 0.0f,            1.0f,0.0f,

    //        // Front face             // Front face              // Front face             // Front face
    //        1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,         0.0f, 0.0f, 1.0f,             1.0f,1.0f,
    //        -1.0f, 1.0f, 1.0f,       1.0f,0.0f,0.0f,         0.0f, 0.0f, 1.0f,             0.0f,1.0f,
    //        -1.0f, -1.0f, 1.0f,      1.0f,0.0f,0.0f,         0.0f, 0.0f, 1.0f,             0.0f,0.0f,
    //        1.0f, -1.0f, 1.0f,       1.0f,0.0f,0.0f,         0.0f, 0.0f, 1.0f,             1.0f,0.0f,

    //        // Back face              // Back face              // Back face              // Back face
    //        1.0f, 1.0f, -1.0f,       0.0f,1.0f,1.0f,         0.0f, 0.0f, -1.0f,             1.0f,1.0f,
    //        -1.0f, 1.0f, -1.0f,      0.0f,1.0f,1.0f,         0.0f, 0.0f, -1.0f,             0.0f,1.0f,
    //        -1.0f, -1.0f, -1.0f,     0.0f,1.0f,1.0f,         0.0f, 0.0f, -1.0f,             0.0f,0.0f,
    //        1.0f, -1.0f, -1.0f,      0.0f,1.0f,1.0f,         0.0f, 0.0f, -1.0f,             1.0f,0.0f,

    //        // Right face              // Right face          	// Right face              // Right face
    //        1.0f, 1.0f, -1.0f,      1.0f,0.0f,1.0f,         1.0f, 0.0f, 0.0f,               1.0f,1.0f,
    //        1.0f, 1.0f, 1.0f,       1.0f,0.0f,1.0f,         1.0f, 0.0f, 0.0f,               0.0f,1.0f,
    //        1.0f, -1.0f, 1.0f,      1.0f,0.0f,1.0f,         1.0f, 0.0f, 0.0f,               0.0f,0.0f,
    //        1.0f, -1.0f, -1.0f,     1.0f,0.0f,1.0f,         1.0f, 0.0f, 0.0f,               1.0f,0.0f,

    //        // Left face                // Left face           // Left face                // Left face
    //        -1.0f, 1.0f, 1.0f,      1.0f,1.0f,0.0f,         -1.0f, 0.0f, 0.0f,              1.0f,1.0f,
    //        -1.0f, 1.0f, -1.0f,     1.0f,1.0f,0.0f,         -1.0f, 0.0f, 0.0f,              0.0f,1.0f,
    //        -1.0f, -1.0f, -1.0f,    1.0f,1.0f,0.0f,         -1.0f, 0.0f, 0.0f,              0.0f,0.0f,
    //        -1.0f, -1.0f, 1.0f,     1.0f,1.0f,0.0f,         -1.0f, 0.0f, 0.0f,              1.0f,0.0f
    //    
    //    
    //    };

    const GLfloat squarePosition[] =
    {
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    const GLfloat squareNormals[] =
    {
        0.0f, 0.0f, 1.0f,  // top-right of front
        0.0f, 0.0f, 1.0f, // top-left of front
        0.0f, 0.0f, 1.0f, // bottom-left of front
        0.0f, 0.0f, 1.0f  // bottom-right of front
    };

        // VAO AND VBO RELATED CODE
	//// vao_Cube
	//glGenVertexArrays(1, &vao_Cube);
	//glBindVertexArray(vao_Cube);

	//glGenBuffers(1, &vbo_Cube);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubePCNT), cubePCNT, GL_STATIC_DRAW); // sizeof(PCNT) is nothing but 11 * 24 * sizeof(float) or 264*sizeof(float)
	//
	//// Position
	//glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(0));
	//glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

	//// Color
	//glVertexAttribPointer(DOMAIN_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_COLOR);

	//// Normal
	//glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

	//// TexCoord
	//glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);

	//glBindVertexArray(0);

    glGenVertexArrays(1, &vao_CloudQuad);
    glBindVertexArray(vao_CloudQuad);

    glGenBuffers(1, &vbo_CloudQuadPosition);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_CloudQuadPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_CloudQuadNormal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_CloudQuadNormal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareNormals), squareNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return(0);
}

//void displayCube(void)
//{
//
//    // Code
//    glBindVertexArray(vao_Cube);
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
//	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
//	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
//	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
//	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
//
//	glBindVertexArray(0);
//
//}

void displayCloudQuad(void)
{
    // code
    glBindVertexArray(vao_CloudQuad);

    // Here there should be drawing code
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

void uninitializeGeometry(void)
{

    // Code
 //   if (vbo_Cube) {

	//	glDeleteBuffers(1, &vbo_Cube);
	//	vbo_Cube = 0;

	//}

	//if (vao_Cube) {

	//	glDeleteVertexArrays(1, &vao_Cube);
	//	vao_Cube = 0;

	//}

    if (vbo_CloudQuadNormal)
    {
        glDeleteBuffers(1, &vbo_CloudQuadNormal);
        vbo_CloudQuadNormal = 0;
    }
    if (vbo_CloudQuadPosition)
    {
        glDeleteBuffers(1, &vbo_CloudQuadPosition);
        vbo_CloudQuadPosition = 0;
    }
    // deleting created vertex array objects
    if (vao_CloudQuad)
    {
        glDeleteVertexArrays(1, &vao_CloudQuad);
        vao_CloudQuad = 0;
    }
}


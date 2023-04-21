#include "../../inc/helper/common.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/Sphere.h"

// cube
GLuint vao_Cube;
GLuint vbo_Cube;

// Cubemap
GLuint vao_cubemap;
GLuint vbo_cubemap;

// pyramid
GLuint vao_pyramid; 
GLuint vbo_pyramid;

// quad
GLuint vao_quad; 
GLuint vbo_quad;

// triangle
GLuint vao_triangle; 
GLuint vbo_triangle;


// sphere
static sphere::Mesh objSphere;

void initializeCube(void)
{
    const GLfloat cubePNT[] = 
    {
                                                //PNT
        // positions                       //normals                   //texture

        // Top face                          // Top face              // Top face
        1.0f, 1.0f, -1.0f,               0.0f, 1.0f, 0.0f,            1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,              0.0f, 1.0f, 0.0f,            0.0f,1.0f,
        -1.0f, 1.0f, 1.0f,               0.0f, 1.0f, 0.0f,            0.0f,0.0f,
        1.0f, 1.0f, 1.0f,                0.0f, 1.0f, 0.0f,            1.0f,0.0f,

        // Bottom face                      // Bottom face            // Bottom face
        1.0f, -1.0f, -1.0f,             0.0f, -1.0f, 0.0f,            1.0f,1.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, -1.0f, 0.0f,            0.0f,1.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, -1.0f, 0.0f,            0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, -1.0f, 0.0f,            1.0f,0.0f,

        // Front face                       // Front face             // Front face
        1.0f, 1.0f, 1.0f,               0.0f, 0.0f, 1.0f,             1.0f,1.0f,
        -1.0f, 1.0f, 1.0f,              0.0f, 0.0f, 1.0f,             0.0f,1.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, 0.0f, 1.0f,             0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, 0.0f, 1.0f,             1.0f,0.0f,

        // Back face                       // Back face              // Back face
        1.0f, 1.0f, -1.0f,              0.0f, 0.0f, -1.0f,             1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,             0.0f, 0.0f, -1.0f,             0.0f,1.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, 0.0f, -1.0f,             0.0f,0.0f,
        1.0f, -1.0f, -1.0f,             0.0f, 0.0f, -1.0f,             1.0f,0.0f,

        // Right face                    	// Right face              // Right face
        1.0f, 1.0f, -1.0f,             1.0f, 0.0f, 0.0f,               1.0f,1.0f,
        1.0f, 1.0f, 1.0f,              1.0f, 0.0f, 0.0f,               0.0f,1.0f,
        1.0f, -1.0f, 1.0f,             1.0f, 0.0f, 0.0f,               0.0f,0.0f,
        1.0f, -1.0f, -1.0f,            1.0f, 0.0f, 0.0f,               1.0f,0.0f,

        // Left face                      // Left face                // Left face
        -1.0f, 1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,              1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,            -1.0f, 0.0f, 0.0f,              0.0f,1.0f,
        -1.0f, -1.0f, -1.0f,           -1.0f, 0.0f, 0.0f,              0.0f,0.0f,
        -1.0f, -1.0f, 1.0f,            -1.0f, 0.0f, 0.0f,              1.0f,0.0f
        
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_Cube);
	glBindVertexArray(vao_Cube);

	glGenBuffers(1, &vbo_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePNT), cubePNT, GL_STATIC_DRAW); // sizeof(PNT) is nothing but 8 * 24 * sizeof(float) or 264*sizeof(float)
	
	// Position
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);


	// Normal
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

	// TexCoord
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);

	glBindVertexArray(0);
    
}

void initializeCubemap(void)
{
    const GLfloat cubePosition[] = 
    {
		// right
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		
		// top
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		
		// bottom
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		
		// front
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		
		// back
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f

    };

    // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_cubemap);
	glBindVertexArray(vao_cubemap);

	glGenBuffers(1, &vbo_cubemap);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cubemap);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePosition), cubePosition, GL_STATIC_DRAW); // sizeof(PNT) is nothing but 8 * 24 * sizeof(float) or 264*sizeof(float)
	
	// Position
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
    
}

void initializeTriangle(void)
{
    const GLfloat trianglePNT[] = 
    {
                                                //PNT
        // positions                  //normals                    //texture

        0.0f, 1.0f, 0.0f,         0.0f, 0.447214f, 0.894427f,        0.5, 1.0, // front-top 
        -1.0f, -1.0f, 0.0f,       0.0f, 0.447214f, 0.894427f,        0.0, 0.0, // front-left
        1.0f, -1.0f, 0.0f,        0.0f, 0.447214f, 0.894427f,        1.0, 0.0, // front-right
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_triangle);
	glBindVertexArray(vao_triangle);

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePNT), trianglePNT, GL_STATIC_DRAW); // sizeof(PNT) is nothing but 8 * 24 * sizeof(float) or 264*sizeof(float)
	
	// Position
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

	// Normal
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

	// TexCoord
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);

	glBindVertexArray(0);
    
}


void initializeQuad(void)
{
    const GLfloat quadPNT[] = 
    {
                                                //PNT
        // positions                     //normals                   //texture

        // Front face                     // Front face             // Front face
        1.0f, 1.0f, 0.0f,             0.0f, 0.0f, 1.0f,             1.0f,1.0f,
        -1.0f, 1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             0.0f,1.0f,
        -1.0f, -1.0f, 0.0f,           0.0f, 0.0f, 1.0f,             0.0f,0.0f,
        1.0f, -1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             1.0f,0.0f,
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_quad);
	glBindVertexArray(vao_quad);

	glGenBuffers(1, &vbo_quad);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPNT), quadPNT, GL_STATIC_DRAW); // sizeof(PNT) is nothing but 8 * 24 * sizeof(float) or 264*sizeof(float)
	
	// Position
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

	// Normal
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

	// TexCoord
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);

	glBindVertexArray(0);
    
}


void initializePyramid(void)
{
    const GLfloat pyramidPNT[] = 
    {
                                                //PNT
        // positions                  //normals                   //texture

        // front            
        0.0f, 1.0f, 0.0f,         0.0f, 0.447214f, 0.894427f,        0.5, 1.0, // front-top 
        -1.0f, -1.0f, 1.0f,       0.0f, 0.447214f, 0.894427f,        0.0, 0.0, // front-left
        1.0f, -1.0f, 1.0f,        0.0f, 0.447214f, 0.894427f,        1.0, 0.0, // front-right
        
        // right 
        0.0f, 1.0f, 0.0f,         0.894427f, 0.447214f, 0.0f,        0.5, 1.0, // right-top 
        1.0f, -1.0f, 1.0f,        0.894427f, 0.447214f, 0.0f,        1.0, 0.0, // right-left
        1.0f, -1.0f, -1.0f,       0.894427f, 0.447214f, 0.0f,        0.0, 0.0, // right-right
        
        // back 
        0.0f, 1.0f, 0.0f,         0.0f, 0.447214f, -0.894427f,        0.5, 1.0, // back-top 
        1.0f, -1.0f, -1.0f,       0.0f, 0.447214f, -0.894427f,        1.0, 0.0, // back-left
        -1.0f, -1.0f, -1.0f,      0.0f, 0.447214f, -0.894427f,        0.0, 0.0, // back-right
        
        // left 
        0.0f, 1.0f, 0.0f,         -0.894427f, 0.447214f, 0.0f,        0.5, 1.0, // left-top 
        -1.0f, -1.0f, -1.0f,      -0.894427f, 0.447214f, 0.0f,        0.0, 0.0, // left-left
        -1.0f, -1.0f, 1.0f,       -0.894427f, 0.447214f, 0.0f,        1.0, 0.0 // left-right
    
    
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_pyramid);
	glBindVertexArray(vao_pyramid);

	glGenBuffers(1, &vbo_pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPNT), pyramidPNT, GL_STATIC_DRAW); // sizeof(PNT) is nothing but 8 * 24 * sizeof(float) or 264*sizeof(float)
	
	// Position
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

	// Normal
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);

	// TexCoord
	glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);

	glBindVertexArray(0);
    
}


void initializeSphere(void)
{
    // code
    sphere::makeSphere(objSphere, 2.0, 30, 30);
}

void displayCube(void)
{
    // Code
    glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);
}

void displayCubemap(void)
{
    // Code
    glBindVertexArray(vao_cubemap);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);
}

void displayTriangle(void)
{
    // Code
    glBindVertexArray(vao_triangle);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void displayQuad(void)
{
    // Code
    glBindVertexArray(vao_quad);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}


void displayPyramid(void)
{
    // Code
    glBindVertexArray(vao_pyramid);

    glDrawArrays(
        GL_TRIANGLES,
        0, // start index
        12 // size - how may vertices to draw
    );

	glBindVertexArray(0);
}



void displaySphere(void)
{
    // code
    objSphere.draw();
}


void uninitializeCube(void)
{
    // Code
    if (vbo_Cube) {

		glDeleteBuffers(1, &vbo_Cube);
		vbo_Cube = 0;
	}

	if (vao_Cube) {

		glDeleteVertexArrays(1, &vao_Cube);
		vao_Cube = 0;
	}
}

void uninitializeCubemap(void)
{
    // Code
    if (vbo_cubemap) {

		glDeleteBuffers(1, &vbo_cubemap);
		vbo_cubemap = 0;
	}

	if (vao_cubemap) {

		glDeleteVertexArrays(1, &vao_cubemap);
		vao_cubemap = 0;
	}
}

void uninitializeTriangle(void)
{
    // Code
    if (vbo_triangle) {

		glDeleteBuffers(1, &vbo_triangle);
		vbo_triangle = 0;
	}

	if (vao_triangle) {

		glDeleteVertexArrays(1, &vao_triangle);
		vao_triangle = 0;
	}
}

void uninitializeQuad(void)
{
    // Code
    if (vbo_quad) {

		glDeleteBuffers(1, &vbo_quad);
		vbo_quad = 0;
	}

	if (vao_quad) {

		glDeleteVertexArrays(1, &vao_quad);
		vao_quad = 0;
	}
}

void uninitializePyramid(void)
{
    // Code
    if (vbo_pyramid) {

		glDeleteBuffers(1, &vbo_pyramid);
		vbo_pyramid = 0;
	}

	if (vao_pyramid) {

		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}
}

void uninitializeSphere(void)
{
    objSphere.cleanupMeshData();
    objSphere.deallocate();
}

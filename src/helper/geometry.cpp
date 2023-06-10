#include "../../inc/helper/common.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/Sphere.h"

// coordinates - x, y, z 
#define NUM_CORDS 3 

// cube
GLuint vao_Cube;
GLuint vbo_Cube;
GLuint vao_invertedNormalCube;
GLuint vao_Cube_TilingTexcoords;


// Cubemap
GLuint vao_cubemap;
GLuint vbo_cubemap;

// pyramid
GLuint vao_pyramid; 
GLuint vbo_pyramid;

// quad
GLuint vao_quad;
GLuint vao_quad_video; 
GLuint vbo_quad;
GLuint vbo_quad_video;




// quad Water
GLuint vao_water_quad;
GLuint vbo_water_quad;

// triangle
GLuint vao_triangle; 
GLuint vbo_triangle;

// sphere
static sphere::Mesh objSphere;
static sphere::Mesh objSphereAtmos;

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

void initializeCubeWithTilingTexcoords(void)
{
    const GLfloat cubePNT[] = 
    {
                                                //PNT
        // positions                       //normals                   //texture

        // Top face                          // Top face              // Top face
        1.0f, 1.0f, -1.0f,               0.0f, 1.0f, 0.0f,            20.0f,20.0f,
        -1.0f, 1.0f, -1.0f,              0.0f, 1.0f, 0.0f,            0.0f,20.0f,
        -1.0f, 1.0f, 1.0f,               0.0f, 1.0f, 0.0f,            0.0f,0.0f,
        1.0f, 1.0f, 1.0f,                0.0f, 1.0f, 0.0f,            20.0f,0.0f,

        // Bottom face                      // Bottom face            // Bottom face
        1.0f, -1.0f, -1.0f,             0.0f, -1.0f, 0.0f,            20.0f,20.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, -1.0f, 0.0f,            0.0f,20.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, -1.0f, 0.0f,            0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, -1.0f, 0.0f,            20.0f,0.0f,

        // Front face                       // Front face             // Front face
        1.0f, 1.0f, 1.0f,               0.0f, 0.0f, 1.0f,             20.0f,20.0f,
        -1.0f, 1.0f, 1.0f,              0.0f, 0.0f, 1.0f,             0.0f,20.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, 0.0f, 1.0f,             0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, 0.0f, 1.0f,             20.0f,0.0f,

        // Back face                       // Back face              // Back face
        1.0f, 1.0f, -1.0f,              0.0f, 0.0f, -1.0f,             20.0f,20.0f,
        -1.0f, 1.0f, -1.0f,             0.0f, 0.0f, -1.0f,             0.0f,20.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, 0.0f, -1.0f,             0.0f,0.0f,
        1.0f, -1.0f, -1.0f,             0.0f, 0.0f, -1.0f,             20.0f,0.0f,

        // Right face                    	// Right face              // Right face
        1.0f, 1.0f, -1.0f,             1.0f, 0.0f, 0.0f,               20.0f,20.0f,
        1.0f, 1.0f, 1.0f,              1.0f, 0.0f, 0.0f,               0.0f,20.0f,
        1.0f, -1.0f, 1.0f,             1.0f, 0.0f, 0.0f,               0.0f,0.0f,
        1.0f, -1.0f, -1.0f,            1.0f, 0.0f, 0.0f,               20.0f,0.0f,

        // Left face                      // Left face                // Left face
        -1.0f, 1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,              20.0f,20.0f,
        -1.0f, 1.0f, -1.0f,            -1.0f, 0.0f, 0.0f,              0.0f,20.0f,
        -1.0f, -1.0f, -1.0f,           -1.0f, 0.0f, 0.0f,              0.0f,0.0f,
        -1.0f, -1.0f, 1.0f,            -1.0f, 0.0f, 0.0f,              20.0f,0.0f
        
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_Cube_TilingTexcoords);
	glBindVertexArray(vao_Cube_TilingTexcoords);

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

void initializeInvertedNormalCube(void)
{
    const GLfloat cubePNT[] = 
    {
                                            //PNT
        // positions                       //normals                   //texture

        // Top face                          // Top face              // Top face
        1.0f, 1.0f, -1.0f,               0.0f, -1.0f, 0.0f,            1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,              0.0f, -1.0f, 0.0f,            0.0f,1.0f,
        -1.0f, 1.0f, 1.0f,               0.0f, -1.0f, 0.0f,            0.0f,0.0f,
        1.0f, 1.0f, 1.0f,                0.0f, -1.0f, 0.0f,            1.0f,0.0f,

        // Bottom face                      // Bottom face            // Bottom face
        1.0f, -1.0f, -1.0f,             0.0f, 1.0f, 0.0f,            1.0f,1.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, 1.0f, 0.0f,            0.0f,1.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, 1.0f, 0.0f,            0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, 1.0f, 0.0f,            1.0f,0.0f,

        // Front face                       // Front face             // Front face
        1.0f, 1.0f, 1.0f,               0.0f, 0.0f, -1.0f,             1.0f,1.0f,
        -1.0f, 1.0f, 1.0f,              0.0f, 0.0f, -1.0f,             0.0f,1.0f,
        -1.0f, -1.0f, 1.0f,             0.0f, 0.0f, -1.0f,             0.0f,0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, 0.0f, -1.0f,             1.0f,0.0f,

        // Back face                       // Back face              // Back face
        1.0f, 1.0f, -1.0f,              0.0f, 0.0f, 1.0f,             1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,             0.0f, 0.0f, 1.0f,             0.0f,1.0f,
        -1.0f, -1.0f, -1.0f,            0.0f, 0.0f, 1.0f,             0.0f,0.0f,
        1.0f, -1.0f, -1.0f,             0.0f, 0.0f, 1.0f,             1.0f,0.0f,

        // Right face                    	// Right face              // Right face
        1.0f, 1.0f, -1.0f,             -1.0f, 0.0f, 0.0f,               1.0f,1.0f,
        1.0f, 1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,               0.0f,1.0f,
        1.0f, -1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,               0.0f,0.0f,
        1.0f, -1.0f, -1.0f,            -1.0f, 0.0f, 0.0f,               1.0f,0.0f,

        // Left face                      // Left face                // Left face
        -1.0f, 1.0f, 1.0f,             1.0f, 0.0f, 0.0f,              1.0f,1.0f,
        -1.0f, 1.0f, -1.0f,            1.0f, 0.0f, 0.0f,              0.0f,1.0f,
        -1.0f, -1.0f, -1.0f,           1.0f, 0.0f, 0.0f,              0.0f,0.0f,
        -1.0f, -1.0f, 1.0f,            1.0f, 0.0f, 0.0f,              1.0f,0.0f
        
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_invertedNormalCube);
	glBindVertexArray(vao_invertedNormalCube);

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

int initializeInstancedQuad(quad_instancing_buffers_t& instBuffers, int numInstances, GLfloat instancePositions[])
{
    // const GLfloat quadPNT[] = 
    // {
    //                                             //PCNT
    //     // positions                     //normals                   //texture

    //     // Front face                     // Front face             // Front face
    //     1.0f, 1.0f, 0.0f,             0.0f, 0.0f, 1.0f,             1.0f, 1.0f,
    //     -1.0f, 1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             0.0f, 1.0f,
    //     -1.0f, -1.0f, 0.0f,           0.0f, 0.0f, 1.0f,             0.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             1.0f, 0.0f,
    // };


    GLfloat square_vertices[] =
    {
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    const GLfloat instance_texcoords[] = 
    {
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    GLuint offset = 0;

    // VAO and VBO related code
    glGenVertexArrays(1, &instBuffers.vao_quadInstanced);
    glBindVertexArray(instBuffers.vao_quadInstanced);
    {
        // Recording
        // tells OpenGL to use vbo (vertexBufferObject) whenever it needs the GL_ARRAY_BUFFER.
        glGenBuffers(1, &instBuffers.vbo_quadInstanced);
        // binding to particular type of target - buffer which holds array
        glBindBuffer(GL_ARRAY_BUFFER, instBuffers.vbo_quadInstanced);

        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(square_vertices) + 
            (sizeof(GLfloat) * 4 * numInstances), // float * 4 (x,y,z,w) * num inst
            NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(square_vertices), square_vertices);
        offset += sizeof(square_vertices);
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * 4 * numInstances, instancePositions);
        offset += sizeof(GLfloat) * 4 * numInstances;
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_INSTANCE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square_vertices)));
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_INSTANCE_POSITION);
        glVertexAttribDivisor(DOMAIN_ATTRIBUTE_INSTANCE_POSITION, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);

///////////////////////////////////////////////////////////
        glGenBuffers(1, &instBuffers.vbo_texcoords);
        // binding to particular type of target - buffer which holds array
        glBindBuffer(GL_ARRAY_BUFFER, instBuffers.vbo_texcoords);

        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(instance_texcoords) , 
            instance_texcoords, GL_STATIC_DRAW);
        
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    // recording complete
    glBindVertexArray(0);
	return (0);
}

void initializeWaterQuad(void)
{
	const GLfloat quadPNT[] =
	{
				//PNT
		// positions                     //normals                   //texture

		// Front face                     // Front face             // Front face
		1.0f, 0.0f, 1.0f,             0.0f, 1.0f, 0.0f,             1.0f,1.0f,
		-1.0f, 0.0f, 1.0f,            0.0f, 1.0f, 0.0f,             0.0f,1.0f,
		-1.0f, 0.0f, -1.0f,           0.0f, 1.0f, 0.0f,             0.0f,0.0f,
		1.0f, 0.0f, -1.0f,            0.0f, 1.0f, 0.0f,             1.0f,0.0f
	};

	// VAO AND VBO RELATED CODE
	// vao_Cube
	glGenVertexArrays(1, &vao_water_quad);
	glBindVertexArray(vao_water_quad);

	glGenBuffers(1, &vbo_water_quad);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_water_quad);
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

void initializeQuadForVideo(void)
{
    const GLfloat quadPNT[] = 
    {
                                                //PNT
        // positions                     //normals                   //texture

        // Front face                     // Front face             // Front face
        1.0f, 1.0f, 0.0f,             0.0f, 0.0f, 1.0f,             1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,           0.0f, 0.0f, 1.0f,             0.0f, 1.0f,
        1.0f, -1.0f, 0.0f,            0.0f, 0.0f, 1.0f,             1.0f, 1.0f,
    };

        // VAO AND VBO RELATED CODE
	// vao_Cube
	
	glGenVertexArrays(1, &vao_quad_video);
	glBindVertexArray(vao_quad_video);
	glGenBuffers(1, &vbo_quad_video);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_video);
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

void initializeSphere(float radius, int slices, int stacks)
{
    // code
    sphere::makeSphere(objSphere, radius, slices, stacks);
}

void initializeSphereAtmos(float radius, int slices, int stacks)
{
	// code
	sphere::makeSphere(objSphereAtmos, radius, slices, stacks);
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

void displayCubeWithTilingTexcoords(void)
{
    // Code
    glBindVertexArray(vao_Cube_TilingTexcoords);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);
}

void displayRoom(GLuint* roomWalls)
{
    // Code
    glBindVertexArray(vao_invertedNormalCube);

	glBindTextureUnit(0, roomWalls[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTextureUnit(0, roomWalls[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glBindTextureUnit(0, roomWalls[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glBindTextureUnit(0, roomWalls[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glBindTextureUnit(0, roomWalls[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindTextureUnit(0, roomWalls[3]);

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

void displayVideoQuad(void)
{
    // Code
    glBindVertexArray(vao_quad_video);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}

void displayInstancedQuads(quad_instancing_buffers_t& instBuffers, int numInstances)
{
    glBindVertexArray(instBuffers.vao_quadInstanced);
    
    // drawing code of 12 lac lines
    glDrawArraysInstanced(
        GL_TRIANGLE_FAN,
        0, // start index
        4, // size - how may vertices to draw
        numInstances  // how many instances to draw
    );
    
    // unbind vao
    glBindVertexArray(0);
}

void displayWaterQuad(void)
{
	// Code
	glBindVertexArray(vao_water_quad);

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

void displaySphere(GLfloat* color)
{
    // code
	objSphere.draw(color);
}

void displaySphereAtmos(GLfloat* color)
{
	// code
	objSphereAtmos.draw(color);
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

void uninitializeVideoQuad(void)
{
    // Code
    if (vbo_quad_video) {

		glDeleteBuffers(1, &vbo_quad_video);
		vbo_quad_video = 0;
	}

	if (vao_quad_video) {

		glDeleteVertexArrays(1, &vao_quad_video);
		vao_quad_video = 0;
	}
}

void uninitializeInstancedQuads(quad_instancing_buffers_t& instBuffers)
{
    if (instBuffers.vbo_texcoords) {

		glDeleteBuffers(1, &instBuffers.vbo_texcoords);
		instBuffers.vbo_texcoords = 0;
	}
    if (instBuffers.vbo_quadInstanced) {

		glDeleteBuffers(1, &instBuffers.vbo_quadInstanced);
		instBuffers.vbo_quadInstanced = 0;
	}

	if (instBuffers.vao_quadInstanced) {

		glDeleteVertexArrays(1, &instBuffers.vao_quadInstanced);
		instBuffers.vao_quadInstanced = 0;
	}
}

void uninitializeWaterQuad(void)
{
	// Code
	if (vbo_water_quad) {

		glDeleteBuffers(1, &vbo_water_quad);
		vbo_water_quad = 0;
	}

	if (vao_water_quad) {

		glDeleteVertexArrays(1, &vao_water_quad);
		vao_water_quad = 0;
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

void uninitializeSphereAtmos(void)
{
	objSphereAtmos.cleanupMeshData();
	objSphereAtmos.deallocate();
}


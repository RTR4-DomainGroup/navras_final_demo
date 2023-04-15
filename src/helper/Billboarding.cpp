#include "../../inc/common.h"
#include "../../inc/geometry.h"
#include "../../inc/texture_loader.h"

// vertex array object
GLuint vao_instancing;
GLuint vao_basequad;

// vertex buffer object
GLuint vbo_position; 
GLuint vbo_position_basequad; 
GLuint vbo_texcoords; 

TEXTURE texture_grass;
TEXTURE texture_flower;


int initializeBillboarding(void)
{
	// local variable
	int retVal = 0;

    // Code
    // declaration of vertex data arrays
    // In PP: glVertex3f, glTexCords, etx will be replaced by arrays
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

    // This array (vertex buffer) we need to push to pipeline (GPU memory)
    // memory mapped IO - vbo - accessing place from GPU memory from CPU memory

    GLuint offset = 0;

    // vao_instancing and VBO related code
    glGenVertexArrays(1, &vao_instancing);
    glBindVertexArray(vao_instancing);
    {
        // Recording
        // tells OpenGL to use vbo (vertexBufferObject) whenever it needs the GL_ARRAY_BUFFER.
        glGenBuffers(1, &vbo_position);
        // binding to particular type of target - buffer which holds array
        glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(square_vertices) + 
            (sizeof(instance_positions[0]) * 4 * NO_OF_INSTANCES), // float * 4 (x,y,z,w) * num inst
            NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(square_vertices), square_vertices);
        offset += sizeof(square_vertices);
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_positions[0]) * 4 * NO_OF_INSTANCES, instance_positions);
        offset += sizeof(instance_positions[0]) * 4 * NO_OF_INSTANCES;
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_INSTANCE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square_vertices)));
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_INSTANCE_POSITION);
        glVertexAttribDivisor(DOMAIN_ATTRIBUTE_INSTANCE_POSITION, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);

///////////////////////////////////////////////////////////
        glGenBuffers(1, &vbo_texcoords);
        // binding to particular type of target - buffer which holds array
        glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);

        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(instance_texcoords) , 
            instance_texcoords, GL_STATIC_DRAW);
        
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // recording complete
    glBindVertexArray(0);

///////////////////////////////////////////////////////////
    // base quad
    glGenVertexArrays(1, &vao_basequad);
    glBindVertexArray(vao_basequad);
    {
        // Recording
        // tells OpenGL to use vbo (vertexBufferObject) whenever it needs the GL_ARRAY_BUFFER.
        glGenBuffers(1, &vbo_position_basequad);
        // binding to particular type of target - buffer which holds array
        glBindBuffer(GL_ARRAY_BUFFER, vbo_position_basequad);

        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(square_vertices), 
            square_vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    glBindVertexArray(0);

	// depth related code
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Load The Texture
	char imagefile[64] = {0};

	sprintf(imagefile, "%sgrass.png", TEXTURE_DIR);
	if (LoadGLTextureData_UsingSOIL(&texture_grass, imagefile) == GL_FALSE)
	{
        LOG("Texture loading failed for image %s\n", imagefile);
        return (-6);
    }

	return (retVal);
}

void displayBillboarding(void)
{
    // Code

    // transformations
    mat4 modelMatrix = mat4::identity();
    mat4 translationMatrix = mat4::identity();
    mat4 rotationMatrix = mat4::identity();
    mat4 viewMatrix = mat4::identity(); 
    mat4 scaleMatrix = mat4::identity(); 

//////////////////////////////
    // base quad

    // repeat the recoded casset again
    glBindVertexArray(vao_basequad);

    translationMatrix = mat4::identity(); 
    modelMatrix = mat4::identity();
    viewMatrix = vmath::lookat(camera.Eye, camera.Center, camera.Up);

    translationMatrix = vmath::translate(0.0f, -6.0f, 0.0f);
    // translationMatrix = vmath::translate(tf.x, tf.y,tf.z);
    // translationMatrix = vmath::translate(19.0f, -5.0f, 1.64f);
    mat4 rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
    scaleMatrix = vmath::scale(40.0f, 1.0f, 40.0f);
    // scaleMatrix = vmath::scale(tf.x, tf.y,tf.z);

    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix_x;

    // send to shader
    glUniformMatrix4fv(
        modelMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        modelMatrix
    );

    glUniformMatrix4fv(
        viewMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        viewMatrix
    );

    glUniformMatrix4fv(
        projectionMatrixUniform, // which uniform4
        1, // 
        GL_FALSE,
        perspectiveProjectionMatrix
    );
    glUniform1i(billboardingUniform, 0);

    glDrawArrays(
        GL_TRIANGLE_FAN,
        0, // start index
        4 // size - how may vertices to draw
    );


    //
    glBindVertexArray(0);


//////////////////////////////////////////

    translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
    if(texture_grass.height > texture_grass.width)
        scaleMatrix = vmath::scale(texture_grass.width/(GLfloat)texture_grass.height, 1.0f, 1.0f);
    else
        scaleMatrix = vmath::scale(1.0f, texture_grass.height/(GLfloat)texture_grass.width, 1.0f);
    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

    // send to shader
    glUniformMatrix4fv(
        modelMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        modelMatrix
    );
    glUniformMatrix4fv(
        viewMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        viewMatrix
    );
    glUniformMatrix4fv(
        projectionMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        perspectiveProjectionMatrix
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_grass.id);
    glUniform1i(textureSamplerUniform, 0);

    if(bBillboardingEnabled)
        glUniform1i(billboardingUniform, 1);
    else
        glUniform1i(billboardingUniform, 0);

    // repeat the recoded casset again
    glBindVertexArray(vao_instancing);

    // drawing code of 12 lac lines
    glDrawArraysInstanced(
        GL_TRIANGLE_FAN,
        0, // start index
        4, // size - how may vertices to draw
        NO_OF_INSTANCES  // how many instances to draw
    );

    // unbind vao
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

float angleForYRotation = 0.0f;
float radius = 30.0f;

// float radius = 0.0f;
void update_ogl(void)
{

    angleForYRotation = angleForYRotation + 0.1f;
    if(angleForYRotation >= 360.0)
        angleForYRotation = 0.0;

    float xDistance = 0.0f;
    static float zDistance = 0.0f;
    zDistance = tf_R;

    float radn = DEG2RADN(angleForYRotation);
    camera.Eye[0] = xDistance + (radius * cos(radn));
    camera.Eye[1] = 0;
    camera.Eye[2] = zDistance + (radius * sin(radn));
}


void uninitializeBillboarding(void)
{

    // Code
    // texture
    if(texture_grass.id)
    {
        glDeleteTextures(1, &texture_grass.id);
        texture_grass.id = 0;
    }
    
    // deletion and uninitilization of vbo
    if(vbo_position)
    {
        glDeleteBuffers(1, &vbo_position);
        vbo_position = 0;
    }
    if(vbo_texcoords)
    {
        glDeleteBuffers(1, &vbo_texcoords);
        vbo_texcoords = 0;
    }

    // deletion and uninitilization of vao
    if(vao_instancing)
    {
        glDeleteVertexArrays(1, &vao_instancing);
        vao_instancing = 0;
    }

    if(vbo_position_basequad)
    {
        glDeleteBuffers(1, &vbo_position_basequad);
        vbo_position_basequad = 0;
    }
    if(vao_basequad)
    {
        glDeleteVertexArrays(1, &vao_basequad);
        vao_basequad = 0;
    }

}


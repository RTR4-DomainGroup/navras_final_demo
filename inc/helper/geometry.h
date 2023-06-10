#pragma once

typedef struct {
    // instanced quads
    GLuint vao_quadInstanced; 
    GLuint vbo_quadInstanced; // PNT
    GLuint vbo_texcoords; // InstancePosition 
} quad_instancing_buffers_t;

void initializeCube(void);
void initializeInvertedNormalCube(void);
void initializeCubeWithTilingTexcoords(void);
void initializeCubemap(void);
void initializeQuad(void);
int initializeInstancedQuad(quad_instancing_buffers_t& instBuffers, int numInstances, GLfloat instancePositions[]);
void initializeQuadForVideo(void);
void initializePyramid(void);

void initializeTriangle(void);
void initializeWaterQuad(void);
void initializeQuadForVideo(void);
void initializeSphere(float radius, int slices, int stacks);
void initializeSphereAtmos(float radius, int slices, int stacks);
void initializeTriangle(void );

void displayGeometry(void);
void displayCube(void);
void displayCubeWithTilingTexcoords(void);
void displayRoom(GLuint*);
void displayCubemap(void);
void displaySphere(GLfloat*);
void displaySphereAtmos(GLfloat*);
void displayTriangle(void );
void displayQuad(void );
void displayVideoQuad(void);
void displayInstancedQuads(quad_instancing_buffers_t& instBuffers, int numInstances);
void displayPyramid(void );
void displayWaterQuad(void);

void uninitializeCube(void);
void uninitializeCubemap(void);
void uninitializePyramid(void);
void uninitializeSphere(void);
void uninitializeSphereAtmos(void);
void uninitializeWaterQuad(void);
void uninitializeQuad(void);
void uninitializeVideoQuad(void);
void uninitializeInstancedQuads(quad_instancing_buffers_t& instBuffers);
void uninitializeTriangle(void);

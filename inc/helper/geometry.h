#pragma once

void initializeCube(void);
void initializeCubemap(void);
void initializeQuad(void);
void initializeInstancedQuad(int numInstances, GLfloat* instancePositions);
void initializeQuadForVideo(void);
void initializePyramid(void);

void initializeTriangle(void);
void initializeWaterQuad(void);
void initializeSphere(float radius, int slices, int stacks);
void initializeSphereAtmos(float radius, int slices, int stacks);
void initializeTriangle(void );

void displayGeometry(void);
void displayCube(void);
void displayCubemap(void);
void displaySphere(GLfloat*);
void displaySphereAtmos(GLfloat*);
void displayTriangle(void );
void displayQuad(void );
void displayInstancedQuads(int numInstances);
void displayPyramid(void );
void displayWaterQuad(void);

void uninitializeCube(void);
void uninitializeCubemap(void);
void uninitializePyramid(void);
void uninitializeSphere(void);
void uninitializeSphereAtmos(void);
void uninitializeWaterQuad(void);
void uninitializeQuad(void);
void uninitializeInstancedQuads(void);
void uninitializeTriangle(void);

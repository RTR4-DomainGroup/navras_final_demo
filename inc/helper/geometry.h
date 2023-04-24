#pragma once

void initializeCube(void);
void initializeCubemap(void);
void initializeQuad(void);
void initializePyramid(void);
void initializeSphere(float radius, int slices, int stacks);
void initializeTriangle(void );

void displayGeometry(void);
void displayCube(void);
void displayCubemap(void);
void displaySphere(GLfloat*);
void displayTriangle(void );
void displayQuad(void );
void displayPyramid(void );

void uninitializeCube(void);
void uninitializeCubemap(void);
void uninitializePyramid(void);
void uninitializeSphere(void);
void uninitializeQuad(void);
void uninitializeTriangle(void);

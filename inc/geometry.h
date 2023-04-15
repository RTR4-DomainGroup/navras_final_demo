#pragma once

void initializeCube(void);
void initializeQuad(void);
void initializeInstancedQuad(int numInstances);
void initializePyramid(void);
void initializeSphere(void);
void initializeTriangle(void );

void displayGeometry(void);
void displayCube(void);
void displaySphere(void);
void displayTriangle(void );
void displayQuad(void );
void displayInstancedQuads(int numInstances);
void displayPyramid(void );

void uninitializeCube(void);
void uninitializePyramid(void);
void uninitializeSphere(void);
void uninitializeQuad(void);
void uninitializInstancedQuads(void);
void uninitializeTriangle(void);


#pragma once
#include <GL/gl.h>
#include "../../inc/helper/geometrytypes.h"
#include "../../inc/helper/vmath.h"

void debug_tranformation(GLbyte charPressed, GLuint keyPressed);
void update_transformations(vmath::mat4* translationMatrix, vmath::mat4* scaleMatrix = NULL, vmath::mat4* rotationMatrix = NULL, TRANFORM* rotateAngles = NULL);
void update_transformations_glm(glm::mat4* translationMatrix, glm::mat4* scaleMatrix = NULL, glm::mat4* rotationMatrix = NULL, TRANFORM* rotateAngles = NULL);
void print_vector(const vmath::vec4& vector);
void print_matrix(const vmath::mat4& matrix);
void print_matrix_glm(const glm::mat4& matrix);
void print_matrices(const vmath::mat4* const translationMatrix, const vmath::mat4* const scaleMatrix = NULL, const vmath::mat4* const rotationMatrix = NULL, const TRANFORM* const vector = NULL);
void print_lookat(void);
void print_transformations(void) ;
void print_camera_transformations(void);

// external variables
// transformation controllers
extern TRANFORM tf_t; // translation
extern TRANFORM tf_s; // scale
extern TRANFORM tf_r; // rotation
extern GLfloat tf_Speed; // transformation speed
extern int tf_Object;


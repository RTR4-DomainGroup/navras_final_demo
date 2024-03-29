#pragma once
#include <GL/gl.h>
#include "../../inc/helper/geometrytypes.h"
#include "../../inc/helper/vmath.h"

void debug_tranformation(GLbyte charPressed, GLuint keyPressed);
void update_transformations(vmath::mat4* translationMatrix = NULL, vmath::mat4* scaleMatrix = NULL, vmath::mat4* rotationMatrix = NULL, TRANFORM* rotateAngles = NULL);
void update_transformations_glm(glm::mat4* translationMatrix = NULL, glm::mat4* scaleMatrix = NULL, glm::mat4* rotationMatrix = NULL, TRANFORM* rotateAngles = NULL);
void print_vector(const vmath::vec4& vector) ;
void print_matrix(const vmath::mat4& matrix);
void print_matrix_glm(const glm::mat4& matrix);
void print_matrices(const vmath::mat4& translationMatrix, const vmath::mat4& scaleMatrix, const vmath::mat4& rotationMatrix, const TRANFORM& vector);
// external variables
// transformation controllers
extern TRANFORM tf_t; // translation
extern TRANFORM tf_s; // scale
extern TRANFORM tf_r; // rotation
extern GLfloat tf_Speed; // transformation speed
extern int tf_Object;

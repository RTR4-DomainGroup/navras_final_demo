#pragma once
#include <GL/gl.h>
#include "../../inc/helper/geometrytypes.h"
#include "../../inc/helper/vmath.h"

void debug_tranformation(void);
void update_transformations(vmath::mat4& translationMatrix, vmath::mat4& scaleMatrix, vmath::mat4& rotationMatrix) ;

// external variables
// transformation controllers
extern TRANFORM tf_t; // translation
extern TRANFORM tf_s; // scale
extern TRANFORM tf_r; // rotation
extern GLfloat tf_Speed; // transformation speed

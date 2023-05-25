#pragma once
#include <GL/gl.h>
#include "../../inc/helper/geometrytypes.h"
void debug_tranformation(void);


// external variables
// transformation controllers
extern TRANFORM tf_t; // translation
extern TRANFORM tf_s; // scale
extern TRANFORM tf_r; // rotation
extern GLfloat tf_Speed; // transformation speed

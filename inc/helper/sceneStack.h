#pragma once 

#include "../scenes/scenes_types.h"

// Global functions declarations
void scenePush(scene_types_t);

scene_types_t scenePop ();

scene_types_t sceneNext();

scene_types_t scenePrev();


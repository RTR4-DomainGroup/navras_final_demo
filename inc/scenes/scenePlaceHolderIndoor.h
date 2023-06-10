#pragma once
typedef void(*SET_CAMERA) (void);
typedef void (* DISPLAY_PASSES_INDOOR) (void);

typedef void(*SET_CAMERA) (void);

typedef void (*DISPLAY_PASSES_INDOOR) ();


int initializeScene_PlaceHolderIndoor(void);
void displayScene_PlaceHolderIndoor(SET_CAMERA, DISPLAY_PASSES_INDOOR, bool);
void updateScene_PlaceHolderIndoor(void);
void uninitializeScene_PlaceHolderIndoor(void);


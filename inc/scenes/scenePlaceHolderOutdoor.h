#pragma once

typedef void(*SET_CAMERA) (void);

typedef void (* DISPLAY_PASSES) (int, bool, bool, bool, int);

int initializeScene_PlaceHolderOutdoor(void);
void displayScene_PlaceHolderOutdoor(SET_CAMERA, DISPLAY_PASSES, bool, bool, bool);
void updateScene_PlaceHolderOutdoor(void);
void uninitializeScene_PlaceHolderOutdoor(void);


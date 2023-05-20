#pragma once

typedef void (*DISPLAY_PASSES)(int,bool,bool,bool,int);

int initializeScene_PlaceHolderOutdoor(void);
void displayScene_PlaceHolderOutdoor(DISPLAY_PASSES);
void updateScene_PlaceHolderOutdoor(void);
void uninitializeScene_PlaceHolderOutdoor(void);


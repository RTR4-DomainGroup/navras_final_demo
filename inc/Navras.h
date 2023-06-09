#pragma once

scene_types_t  getCurrentScene(void);

int eventHandlerNavras(unsigned int iMsg, int wParam);
int initializeNavras(void);
void resizeNavras(int, int);
void displayNavras(void);
void updateNavras(void);
void uninitializeNavras(void);



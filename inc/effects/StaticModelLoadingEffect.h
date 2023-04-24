#pragma once
#include "../../inc/helper/staticmodel.h"

//PROPERTIES OF VERTEX:
typedef struct _STATIC_MODEL
{
	StaticModel* pModel;

}STATIC_MODEL;

void drawStaticModel(STATIC_MODEL staticModel);
void loadStaticModel(const char* path, STATIC_MODEL* staticModel);
void unloadStaticModel(STATIC_MODEL* staticModel);


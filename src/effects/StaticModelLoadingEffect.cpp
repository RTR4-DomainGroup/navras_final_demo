#pragma once

#include "../../inc/effects/StaticModelLoadingEffect.h"

void loadStaticModel(const char* path, STATIC_MODEL* staticModel)
{
	staticModel->pModel = new StaticModel(path, true);
	if (staticModel->pModel == NULL)
	{
		LOG("loadStaticModel() new Model Failed.\n");
	}
}

void drawStaticModel(STATIC_MODEL staticModel)
{
	staticModel.pModel->Draw();
}

void unloadStaticModel(STATIC_MODEL* staticModel)
{
	if (staticModel->pModel)
	{
		delete staticModel->pModel;
		staticModel->pModel = nullptr;
	}
}



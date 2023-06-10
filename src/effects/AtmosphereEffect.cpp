#include "../../inc/effects/AtmosphereEffect.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/geometry.h"

// Atmosphere Scattering
AtmosphericVariables atmosVariables;

int initializeAtmosphere(AtmosphericVariables atmosVariables) {
	
	initializeSphereAtmos(atmosVariables.m_fOuterRadius,100,50);

	return 0;
}

void displayAtmosphere(void) {

	// Code
	float color[3] = {0.0f,0.0f,0.0f};
	displaySphereAtmos(color);
}

void uninitializeAtmosphere(void) {

	uninitializeSphereAtmos();
}

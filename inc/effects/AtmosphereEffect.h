#pragma once
#include "../shaders/AtmosphereShader.h"

struct AtmosphericVariables
{

	int m_nSamples;
	float m_Kr, m_Kr4PI;
	float m_Km, m_Km4PI;
	float m_ESun;
	float m_g;
	float m_fExposure;

	float m_fInnerRadius;
	float m_fOuterRadius;
	float m_fScale;
	float m_fWavelength[3];
	float m_fWavelength4[3];
	float m_fRayleighScaleDepth;
	float m_fMieScaleDepth;

	vec3 m_vLight;
	vec3 m_vLightDirection;

};

int initializeAtmosphere(AtmosphericVariables);
void displayAtmosphere(void);
void uninitializeAtmosphere(void);

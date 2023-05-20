#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include "../../inc/shaders/SkyboxShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/CloudNoiseShader.h"
#include "../../inc/shaders/TerrainShader.h"
#include "../../inc/shaders/StarfieldShader.h"
#include "../inc/shaders/WaterShader.h"
#include "../../inc/shaders/BillboardingShader.h"
#include "../../inc/shaders/HorrizontalBlur.h"
#include "../../inc/shaders/VerticalBlur.h"
#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/shaders/ParticleShader.h"


BOOL initAllShaders(void)
{
    // Variable Declarations

    // Code
    if (initializeFSQuadShader() != 0)
    {
        return FALSE;
    }

    if (initializeAtmosphereShader() != 0)
    {
        return FALSE;
    }

    if (initializeADSShader() != 0)
    {
        return FALSE;
    }

    if (initializeADSDynamicShader() != 0)
    {
        return FALSE;
    }

    if (initializeSkyboxShader() != 0)
    {
        return FALSE;
    }
    
    if(initializeFSQuadShader() != 0)
    {
        return FALSE;
    }

    if (initializeGodraysShader() != 0)
    {
        return FALSE;
    }

    if (initializeTerrainShader() != 0)
    {
        return FALSE;
    }

    if(intializeCloudNoiseShader() != 0)
    {
        return FALSE;
    }

    if (initializeStarfieldShader() != 0)
    {
        return FALSE;
    }

	if(initializeBillboardingShader() != 0)
    {
        return FALSE;
    }

    if (initializeWaterShader() != 0)
    {
        return FALSE;
    }

    if (initialize_horrizontalBlur() != 0)
    {
        return FALSE;
    }
    
    if (initialize_verticalBlur() != 0)
    {
        return FALSE;
    }

    if (initializeParticleShader() != 0)
    {
        return FALSE;
    }

    return TRUE;
}

void uninitializeAllShaders(void)
{
    // Code
    uninitializeParticleShader();
    uninitializeBillboardingShader();
    uninitializeWaterShader();
    uninitializeStarfieldShader();
    uninitializeTerrainShader();
    uninitializeGodraysShader();
    uninitializeSkyboxShader();
    uninitializeADSShader();
    uninitializeADSDynamicShader();
    uninitializeCloudNoiseShader();
    uninitializeAtmosphereShader();
}


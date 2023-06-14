#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include "../../inc/shaders/SkyboxShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/CloudNoiseShader.h"
#include "../../inc/shaders/ErosionNoiseShader.h"
#include "../../inc/shaders/TerrainShader.h"
#include "../../inc/shaders/StarfieldShader.h"
#include "../inc/shaders/WaterShader.h"
#include "../../inc/shaders/BillboardingShader.h"
#include "../../inc/shaders/HorrizontalBlur.h"
#include "../../inc/shaders/VerticalBlur.h"
#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/shaders/ParticleShader.h"
#include "../../inc/shaders/SSAOShader.h"
#include "../../inc/shaders/FontShader.h"
#include "../../inc/shaders/FSVQuadShader.h"


bool initAllShaders(void)
{
    // Variable Declarations

    // Code
    if(initializeFontShader() != 0)
    {
        return false;
    }
    
    if (initializeFSQuadShader() != 0)
    {
        return false;
    }

    if (initializeAtmosphereShader() != 0)
    {
        return false;
    }

    if (initializeADSShader() != 0)
    {
        return false;
    }

    if (initializeADSDynamicShader() != 0)
    {
        return false;
    }

    if (initializeSkyboxShader() != 0)
    {
        return false;
    }
    
#ifndef ENABLE_MULTI_THREADING
    if(initializeFSVQuadShader() != 0)
    {
         return false;
    }
#endif
    if (initializeGodraysShader() != 0)
    {
        return false;
    }

    if (initializeTerrainShader() != 0)
    {
        return false;
    }

    if(intializeCloudNoiseShader() != 0)
    {
        return false;
    }

    if(intializeErosionNoiseShader() != 0)
    {
        return false;
    }

    if (initializeStarfieldShader() != 0)
    {
        return false;
    }

	if(initializeBillboardingShader() != 0)
    {
        return false;
    }

    if (initializeWaterShader() != 0)
    {
        return false;
    }

    if (initialize_horrizontalBlur() != 0)
    {
        return false;
    }
    
    if (initialize_verticalBlur() != 0)
    {
        return false;
    }

    if (initializeParticleShader() != 0)
    {
        return false;
    }
    if (initializeSSAOShader() != 0)
    {
        return false;
    }

    return true;
}

void uninitializeAllShaders(void)
{
    // Code
    uninitializeFSQuadShader();
    uninitialize_horrizontalBlurShader();
    uninitialize_verticalBlurShader();
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
    uninitializeSSAOShader();
#ifndef ENABLE_MULTI_THREADING
    uninitializeFSVQuadShader();
#endif
}


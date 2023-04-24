#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/SkyboxShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/CloudNoiseShader.h"
#include "../../inc/shaders/TerrainShader.h"
#include "../../inc/shaders/StarfieldShader.h"

BOOL initAllShaders(void)
{
    // Variable Declarations

    // Code
    if (initializeADSShader() != 0)
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

    return TRUE;
}

void uninitAllShaders(void)
{
    // Code
    uninitializeStarfieldShader();
    uninitializeTerrainShader();
    uninitializeGodraysShader();
    uninitializeSkyboxShader();
    uninitializeADSShader();
    uninitializeCloudNoiseShader();
}

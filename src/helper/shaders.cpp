#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/TerrainShader.h"

bool initAllShaders(void)
{

    // Variable Declarations
    // Code
    if (initializeFSQuadShader() != 0)
    {
        return FALSE;
    }

    if(initializeADSShader() != 0)
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

    return TRUE;

}


void uninitAllShaders(void)
{

    // Code
    uninitializeTerrainShader();
    uninitializeGodraysShader();
    uninitializeADSShader();

}

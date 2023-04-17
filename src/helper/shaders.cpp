#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/TerrainShader.h"

BOOL initAllShaders(void)
{

    // Variable Declarations

    // Code
    if(initializeADSShader() != 0 &&
        initializeGodraysShader() != 0 &&
        initializeTerrainShader() != 0)
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

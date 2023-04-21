#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/TerrainShader.h"
#include "../../inc/shaders/StarfieldShader.h"
#include "../../inc/shaders/PBRGeometryShader.h"

BOOL initAllShaders(void)
{

    // Variable Declarations

    

    // Code
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
    if (initializePBRGeometryShader() != 0)
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
    uninitializeADSShader();

}

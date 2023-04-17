#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/SkyboxShader.h"
#include "../../inc/shaders/GodraysShader.h"

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
    if (initializeGodraysShader() != 0)
    {
        return FALSE;
    }
    return TRUE;

}

void uninitAllShaders(void)
{
    // Code
    uninitializeGodraysShader();
    uninitializeSkyboxShader();
    uninitializeADSShader();

}

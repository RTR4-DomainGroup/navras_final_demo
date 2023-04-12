#include "../../inc/common.h"
#include "../../inc/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/effects/GodraysShader.h"

BOOL initAllShaders(void)
{

    // Variable Declarations

    // Code
    if(initializeADSShader() != 0 &&
        initializeGodraysShader() != 0)
    {
        return FALSE;
    }

    return TRUE;

}

void uninitAllShaders(void)
{

    // Code
    uninitializeGodraysShader();
    uninitializeADSShader();

}

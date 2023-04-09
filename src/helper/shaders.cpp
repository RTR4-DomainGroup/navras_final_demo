#include "../../inc/common.h"
#include "../../inc/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"

BOOL initAllShaders(void)
{

    // Variable Declarations

    // Code
    if(initializeADSShader() != 0)
    {
        return FALSE;
    }

    return TRUE;

}

void uninitAllShaders(void)
{

    // Code
    uninitializeADSShader();

}

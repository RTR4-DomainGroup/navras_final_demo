#include "../include/common.h"
#include "../include/shaders.h"
#include "../include/shaders/ADSLightShader.h"
#include "../include/shaders/GodraysShader.h"

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
    uninitializeADSShader();

}

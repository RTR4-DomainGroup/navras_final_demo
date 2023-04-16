#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/CloudNoiseShader.h"

BOOL initAllShaders(void)
{
    // Variable Declarations

    // Code
    if(initializeADSShader() != 0 &&
        initializeGodraysShader() != 0 &&
        intializeCloudNoiseShader() != 0)
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
    uninitializeCloudNoiseShader();
}

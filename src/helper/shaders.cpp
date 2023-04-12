#include "../../inc/common.h"
#include "../../inc/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/FSQuadShader.h"

bool initAllShaders(void)
{
    // Variable Declarations

    // Code
    if(initializeADSShader() != 0 &&
        initializeGodraysShader() != 0 && 
        initializeFSQuadShader() != 0)
    {
        return false;
    }

    return true;
}

void uninitAllShaders(void)
{

    // Code
    uninitializeADSShader();

}

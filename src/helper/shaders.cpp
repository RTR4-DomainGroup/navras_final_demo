#include "../../inc/helper/common.h"
#include "../../inc/helper/shaders.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/GodraysShader.h"
#include "../../inc/shaders/BillboardingShader.h"

BOOL initAllShaders(void)
{

    // Variable Declarations

    // Code
    if(initializeADSShader() != 0 &&
        initializeGodraysShader() != 0 &&
        initializeBillboardingShader() != 0
      )
    {
        return FALSE;
    }

    return TRUE;

}

void uninitializeAllShaders(void)
{

    // Code
    uninitializeBillboardingShader();
    uninitializeGodraysShader();
    uninitializeADSShader();

}

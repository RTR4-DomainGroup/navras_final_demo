// This File Will Be Replaced by Scene*.cpp

#include "../include/common.h"

GLuint texture_Marble;

int initializeInterleaved(void)
{

    // Code.
    // Texture
	if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
		//uninitialize();

		return(-1);

	}

}

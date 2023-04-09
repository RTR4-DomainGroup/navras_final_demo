#include "../../inc/common.h"

BOOL LoadGLTexture(GLuint* Texture, TCHAR ImageResourceID[]) 
{

	// Variable Declarations
	HBITMAP hBitMap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	//Code
	hBitMap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitMap) {

		bResult = TRUE;
		GetObject(hBitMap, sizeof(bmp), &bmp);

		//
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//
		glGenTextures(1, Texture);

		//
		glBindTexture(GL_TEXTURE_2D, *Texture);

		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Create The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		// Delete Handle
		DeleteObject(hBitMap);


	}

	return(bResult);

}

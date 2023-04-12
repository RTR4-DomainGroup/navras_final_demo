#include "../../inc/texture_loader.h"

// Texture lib header
#include "../../inc/SOIL.h"


#ifdef __linux__ 
    //linux code goes here
    // dummy macro
#include <sys/time.h>
#include <string.h>
    
GLboolean LoadGLTexture(GLuint* a, TCHAR b[])
{
    return GL_TRUE;
}

#elif _WIN32


#pragma comment(lib, "lib\\SOIL\\SOIL.lib")

// windows code goes here

BOOL LoadGLTexture_UsingBitMap(GLuint* texture, TCHAR imageRecId[])
{
	return LoadGLTexture(texture, imageRecId);
}


//******** ABP - Code For Light On Texture - START ********
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

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, Texture);
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

//******** ABP - Code For Light On Texture - END ********
#else
// architecture not supported
#endif
GLboolean LoadGLTexture_UsingSOIL(GLuint* texture, const char* path)
{
    // variable declarations
    int width, height;
    unsigned char* imageData = NULL;

    // code
    imageData = SOIL_load_image(
        path,
        &width,
        &height,
        NULL, //
        SOIL_LOAD_RGBA // load 3 channel RGB 
    );
    if(imageData == NULL)
    {
		log_printf("SOIL_load_image() failed for file: %s\n", path);
        return (GL_FALSE);
    }

    // step 4
    // Mala pixel store madhye image che packing pahije ki unpacking
    glPixelStorei(
        GL_UNPACK_ALIGNMENT, // image che pixel load kelelya image chya memory madhye unpack pan aligned thew 
        4 // alignment of 4 (in adroid it is 1)
    );

    // generate texture     
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, // magnification - user chya saglyat jawal
        GL_LINEAR
	);

    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR // Jawal asatana Linear, door jatana mipmap, parat jawal yetana linear
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // mipmap level - 0 - use default as per opengl
        GL_RGBA, // How to interpret internal image format 
        width, // from getObjet
        height,
        0, // border width
        GL_RGBA, // microsoft bmp type
        GL_UNSIGNED_BYTE,
        imageData            
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(imageData);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return (GL_TRUE);
}

GLboolean LoadGLTextureData_UsingSOIL(TEXTURE* texture, const char* path)
{
    // variable declarations
    int width, height;
    unsigned char* imageData = NULL;
    
    // code
    imageData = SOIL_load_image(
        path,
        &width,
        &height,
        NULL, //
        SOIL_LOAD_RGBA // load 3 channel RGB 
    );
    if(imageData == NULL)
    {
		log_printf("SOIL_load_image() failed for file: %s\n", path);
        return (GL_FALSE);
    }

    // step 4
    // Mala pixel store madhye image che packing pahije ki unpacking
    glPixelStorei(
        GL_UNPACK_ALIGNMENT, // image che pixel load kelelya image chya memory madhye unpack pan aligned thew 
        4 // alignment of 4 (in adroid it is 1)
    );

    // generate texture     
    glGenTextures(1, &(texture->id));
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, // magnification - user chya saglyat jawal
        GL_LINEAR
    );

    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR // Jawal asatana Linear, door jatana mipmap, parat jawal yetana linear
    );

    glTexImage2D(
        GL_TEXTURE_2D,
        0, // mipmap level - 0 - use default as per opengl
        GL_RGBA, // How to interpret internal image format 
        width, // from getObjet
        height,
        0, // border width
        // Imp: color issue
        GL_RGBA, // openGL compatible format, microsoft bmp type 
        GL_UNSIGNED_BYTE,
        imageData            
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture->width = width;
    texture->height = height;
    
    
    SOIL_free_image_data(imageData);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

    return (GL_TRUE);
}

GLboolean LoadGLTexture_Cubemap(GLuint* texture, const char* path)
{
    // variable declarations
    int width, height;
    
    const char* szCubeFaces[6] = { "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png" };

    GLenum cube[6] = { 
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 
    };

    // code

    // step 4
    // Mala pixel store madhye image che packing pahije ki unpacking
    glPixelStorei(
        GL_UNPACK_ALIGNMENT, // keep image pixels, in loaded image memory as unpack but aligned
        4 // alignment of 4 (in adroid it is 1)
    );

    // generate texture     
    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
    // // magnification - user chya saglyat jawal
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // Jawal asatana Linear, door jatana mipmap, parat jawal yetana linear
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);


    // Load Cube Map images
    for(int i = 0; i < 6; i++)
    {
        unsigned char* imageData = NULL;
        
        char imagepath[64] = {0};
        snprintf(imagepath, sizeof(imagepath), "%s%s", path, szCubeFaces[i]);

		log_printf("SOIL_load_image() failed for file: %s\n", path);

        log_printf("loading image %s for texture\n", imagepath);

        imageData = SOIL_load_image(
            imagepath,
            &width,
            &height,
            NULL, //
            SOIL_LOAD_RGBA // load 3 channel RGB 
        );
        if(imageData == NULL)
        {
            return (GL_FALSE);
        }

        glTexImage2D(
            cube[i], 
            0, 
            GL_RGBA, 
            width, 
            width, 
            0, 
            GL_RGBA, 
            GL_UNSIGNED_BYTE, 
            imageData
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(imageData);
    }

    return (GL_TRUE);
}

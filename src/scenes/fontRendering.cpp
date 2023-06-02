#include "../../inc/helper/camera.h"
#include "../../inc/helper/vmath.h"
#include "../../inc/helper/common.h"
#include "../../inc/scenes/fontRendering.h"
#include "../../inc/shaders/FontShader.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <map>
#include <string>

#pragma comment(lib, "freetype.lib")

extern mat4 perspectiveProjectionMatrix;
// extern int textColorUniform;

FontUniform fontUniforms;

#define STRINGIZE(a) #a

GLuint vao_font;
GLuint vbo_font;

using namespace std;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    vec2   Size;      // Size of glyph
    vec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;


int initializeFont(void) {
	
	// FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        LOG("ERROR::FREETYPE: Could not init FreeType Library");
        return -1;
    }

	// find path to font
    std::string font_name = "res/fonts/arial.ttf";
	// std::string font_name = "arial.ttf";
    if (font_name.empty())
    {
        LOG("ERROR::FREETYPE: Failed to load font_name");
        return -1;
    }
	
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        LOG("ERROR::FREETYPE: Failed to load font as face");
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                LOG("ERROR::FREETYTPE: Failed to load Glyph");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // now store character for later use
            Character character = {
                texture,
                vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &vao_font);
    glGenBuffers(1, &vbo_font);
    glBindVertexArray(vao_font);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_font);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


	// Here starts OpenGL code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Depth Related Changes (for 3D)
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	// enable texture
	glEnable(GL_TEXTURE_2D);


	return(0);
}

void displayFont(void) {

    void RenderText(std::string, float, float, float, vec4);

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader program object
    fontUniforms = useFontShader();


	mat4 modelViewMtrix = mat4::identity();
	mat4 translationMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	

	translationMatrix = translate(-10.0f, 0.0f, -50.0f);
	//scaleMatrix = scale(0.1f, 0.1f, 0.1f);
	//rotationMatrix = rotate(angleC, 1.0f, 1.0f, 1.0f);
	modelViewMtrix = translationMatrix; //* scaleMatrix * rotationMatrix;
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMtrix;

	glUniformMatrix4fv(fontUniforms.uniform_mvp_matrix, 1, GL_FALSE, modelViewProjectionMatrix);



	RenderText("Sampla Project", 0.0f, 0.0f, 0.1f, vec4(1.0f, 0.8f, 0.2f, 1.0));
	//RenderText("(C) LearnOpenGL.com", 0.0f, 0.0f, 0.5f, vec4(0.3, 0.7f, 0.9f,1.0));

	// Unuse the shader program object
	glUseProgram(0);

}

void RenderText(std::string text, GLfloat x, GLfloat y, float scale, vec4 color)
{

	glUniform4f(fontUniforms.textColorUniform, color[0], color[1], color[2], color[3]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao_font);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing[0] * scale;
        float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

        float w = ch.Size[0] * scale;
        float h = ch.Size[1] * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
		
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo_font);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void uninitializeFont(void) {

	if (vao_font) {

		glDeleteVertexArrays(1, &vao_font);
		vao_font = 0;

	}
}

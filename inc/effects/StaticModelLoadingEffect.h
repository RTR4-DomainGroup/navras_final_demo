#pragma once
#include<stdio.h>
#include "../../inc/helper/common.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "../../ext/SOIL/include/SOIL.h"
#include "../../inc/helper/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace vmath;

extern FILE* gpFile;
extern GLuint shaderProgObj;


//PROPERTIES OF VERTEX:
typedef struct _STATIC_MODEL
{
    StaticModel* pModel;

}STATIC_MODEL;

struct StaticModelVertex {
    // position
    vec3 Position;
    // normal
    vec3 Normal;
    // texCoords
    vec2 TexCoords;
    // tangent
    vec3 Tangent;
    // bitangent
    vec3 Bitangent;
};

struct StaticModelTexture {
    unsigned int id;
    string type;
    string path;
};



class Mesh {
public:

    // mesh Data
    vector<StaticModelVertex>       vertices;
    vector<unsigned int> indices;
    vector<StaticModelTexture>      textures;
    unsigned int VAO;

    // render data 
    unsigned int VBO, EBO;

    // constructor
    Mesh(vector<StaticModelVertex> vertices, vector<unsigned int> indices, vector<StaticModelTexture> textures);

    // destructor
    ~Mesh();

    // render the mesh
    void Draw();

    // initializes all the buffer objects/arrays
    void setupMesh();

};

class StaticModel
{
public:

    // model data 
    vector<StaticModelTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh*>    meshes;
    string directory;
    bool gammaCorrection;

    StaticModel();

    // constructor, expects a filepath to a 3D model.
    StaticModel(string const& path, bool gamma = false);

    //destructor
    ~StaticModel();

    // draws the model, and thus all its meshes
    void Draw();

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<StaticModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
void drawStaticModel(STATIC_MODEL staticModel);
void loadStaticModel(const char* path, STATIC_MODEL* staticModel);
void unloadStaticModel(STATIC_MODEL* staticModel);


#pragma once
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/shaders/ADSLightShader.h"


/*############### MESH ###############*/

// constructor
Mesh::Mesh(vector<StaticModelVertex> vertices, vector<unsigned int> indices, vector<StaticModelTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// destructor
Mesh::~Mesh()
{
    vertices.clear();   //vertices will be cleared (clear data)
    vertices.shrink_to_fit();  //free unused memory

    for (int i = 0; i < textures.size(); i++)
    {
        glDeleteTextures(1, &textures[i].id);
    }

    textures.clear();   //textures clear
    textures.shrink_to_fit();

    indices.clear();
    indices.shrink_to_fit();

    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}

// render the mesh
void Mesh::Draw()
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);

        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream

        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream

        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(getADSShaderProgramObject(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(StaticModelVertex), &vertices[0], GL_STATIC_DRAW);


    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(StaticModelVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(StaticModelVertex), (void*)offsetof(StaticModelVertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof(StaticModelVertex), (void*)offsetof(StaticModelVertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TANGENT);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(StaticModelVertex), (void*)offsetof(StaticModelVertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_BITANGENT);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(StaticModelVertex), (void*)offsetof(StaticModelVertex, Bitangent));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //printf("indices = %d", indices);

    glBindVertexArray(0);
}


/*############### MODEL ###############*/

StaticModel::StaticModel()
{

}

// constructor, expects a filepath to a 3D model.
StaticModel::StaticModel(string const& path, bool gamma) : gammaCorrection(gamma)
{
    LOG("------------------------------------------------------------------------------\n");
    LOG("Entry to function = %s\n", __FUNCTION__);
    
    loadModel(path);
   
    LOG("Exit from function = %s\n", __FUNCTION__);
    LOG("------------------------------------------------------------------------------\n");
}

//destructor
StaticModel::~StaticModel()
{

    //MessageBox(NULL, TEXT("~Model"), NULL, MB_OK);
    //fprintf(gpFile, "in ~Model\n");
    textures_loaded.clear();
    textures_loaded.shrink_to_fit();

    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    meshes.clear();
    meshes.shrink_to_fit();
}

// draws the model, and thus all its meshes
void StaticModel::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw();
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void StaticModel::loadModel(string const& path)
{
    LOG("Entry to function = %s\n", __FUNCTION__);
    LOG("obj file path = %s\n", path.c_str());

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        LOG("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    LOG("model directory = %s\n", directory.c_str());

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    LOG("Exit from function = %s\n", __FUNCTION__);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void StaticModel::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh* StaticModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<StaticModelVertex> vertices;
    vector<unsigned int> indices;
    vector<StaticModelTexture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        StaticModelVertex vertex;
        vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector[0] = mesh->mVertices[i].x;
        vector[1] = mesh->mVertices[i].y;
        vector[2] = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector[0] = mesh->mNormals[i].x;
            vector[1] = mesh->mNormals[i].y;
            vector[2] = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec[0] = mesh->mTextureCoords[0][i].x;
            vec[1] = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector[0] = mesh->mTangents[i].x;
            vector[1] = mesh->mTangents[i].y;
            vector[2] = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector[0] = mesh->mBitangents[i].x;
            vector[1] = mesh->mBitangents[i].y;
            vector[2] = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    int index0, index1, index2;
    for (int i = 0; i < indices.size(); i += 3)
    {
        index0 = indices[i];
        index1 = indices[i + 1];
        index2 = indices[i + 2];

        vec3 v0 = vertices[index0].Position;
        vec3 v1 = vertices[index1].Position;
        vec3 v2 = vertices[index2].Position;

        vec2 uv0 = vertices[index0].TexCoords;
        vec2 uv1 = vertices[index1].TexCoords;
        vec2 uv2 = vertices[index2].TexCoords;

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec2 deltaUV1 = uv1 - uv0;
        vec2 deltaUV2 = uv2 - uv0;

        float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

        vec3 tangent, bitangent;

        tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
        tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
        tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);

        bitangent[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
        bitangent[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
        bitangent[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);

        vertices[index0].Tangent += tangent;
        vertices[index1].Tangent += tangent;
        vertices[index2].Tangent += tangent;

        vertices[index0].Bitangent += bitangent;
        vertices[index1].Bitangent += bitangent;
        vertices[index2].Bitangent += bitangent;

    }

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].Tangent = normalize(vertices[i].Tangent);
        vertices[i].Bitangent = normalize(vertices[i].Bitangent);
    }


    // process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<StaticModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<StaticModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<StaticModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<StaticModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    LOG("mesh number = %zu\n", meshes.size());

    // return a mesh object created from the extracted mesh data
    return new Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<StaticModelTexture> StaticModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<StaticModelTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            StaticModelTexture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);

            if (texture.id == 0)
                MessageBox(NULL, NULL, TEXT("Tex not loaded"), MB_OK);

            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //stbi_set_flip_vertically_on_load(true);

    unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        LOG("SUCCESS : texture directory = %s\n", directory.c_str());
        LOG("SUCCESS : texture filename = %s\n", filename.c_str());

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        SOIL_free_image_data(data);
    }
    else
    {
        LOG("ERROR : texture directory = %s\n", directory.c_str());
        LOG("ERROR: texture filename = %s\n", filename.c_str());
        //MessageBox(NULL, TEXT("Texture not loaded"), TEXT("ERROR"), MB_OK);
        SOIL_free_image_data(data);
    }

    return textureID;
}

void loadStaticModel(const char* path, STATIC_MODEL* staticModel)
{
	staticModel->pModel = new StaticModel(path, true);
	if (staticModel->pModel == NULL)
	{
		LOG("loadStaticModel() new Model Failed.\n");
	}
}

void drawStaticModel(STATIC_MODEL staticModel)
{
	staticModel.pModel->Draw();
}

void unloadStaticModel(STATIC_MODEL* staticModel)
{
	if (staticModel->pModel)
	{
		delete staticModel->pModel;
		staticModel->pModel = nullptr;
	}
}



#pragma once
#include "../../inc/helper/common.h"
#include "../../inc/effects/DynamicModelLoadingEffect.h"

#define MAX_BONES 100
GLuint dynamicShaderProgObj;
GLuint finalBonesMatricesUniform[MAX_BONES];

/*############################################################# BONE #############################################################*/

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    :
    m_Name(name),
    m_ID(ID),
    m_LocalTransform(1.0f)
{
    m_NumPositions = channel->mNumPositionKeys;

    for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        float timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
        data.timeStamp = timeStamp;
        m_Positions.push_back(data);
    }

    m_NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
        data.timeStamp = timeStamp;
        m_Rotations.push_back(data);
    }

    m_NumScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        float timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = AssimpGLMHelpers::GetGLMVec(scale);
        data.timeStamp = timeStamp;
        m_Scales.push_back(data);
    }
}

void Bone::Update(float animationTime)
{
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    glm::mat4 scale = InterpolateScaling(animationTime);
    m_LocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::GetLocalTransform() 
{ 
    return m_LocalTransform; 
}

std::string Bone::GetBoneName() 
const { 
    return m_Name; 
}

int Bone::GetBoneID() 
{ 
    return m_ID; 
}

int Bone::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < m_NumPositions - 1; ++index)
    {
        if (animationTime < m_Positions[index + 1].timeStamp)
            return index;
    }
    assert(0);

    return (0);
}

int Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < m_NumRotations - 1; ++index)
    {
        if (animationTime < m_Rotations[index + 1].timeStamp)
            return index;
    }
    assert(0);

    return (0);
}

int Bone::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < m_NumScalings - 1; ++index)
    {
        if (animationTime < m_Scales[index + 1].timeStamp)
            return index;
    }
    assert(0);

    return (0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
    if (1 == m_NumPositions)
        return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
        m_Positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position
        , scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (1 == m_NumRotations)
    {
        auto rotation = glm::normalize(m_Rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
        m_Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation
        , scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);

}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
    if (1 == m_NumScalings)
        return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
        m_Scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale
        , scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}


/*############################################################# ANIMATION #############################################################*/

Animation::Animation(const std::string& animationPath, DynamicModel* model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    auto animation = scene->mAnimations[0];
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    ReadHeirarchyData(m_RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
}

Animation::~Animation()
{
}

Bone* Animation::FindBone(const std::string& name)
{
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
        [&](const Bone& Bone)
        {
            return Bone.GetBoneName() == name;
        }
    );
    if (iter == m_Bones.end()) return nullptr;
    else return &(*iter);
}

inline float Animation::GetTicksPerSecond()
{ 
    return m_TicksPerSecond; 
}

inline float Animation::GetDuration() 
{ 
    return m_Duration; 
}

inline const AssimpNodeData& Animation::GetRootNode() 
{ 
    return m_RootNode;
}

inline const std::map<std::string, BoneInfo>& Animation::GetBoneIDMap()
{
    return m_BoneInfoMap;
}

void Animation::ReadMissingBones(const aiAnimation* animation, DynamicModel& model)
{
    int size = animation->mNumChannels;

    auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

/*############################################################# ANIMATOR #############################################################*/

Animator::Animator(Animation* animation)
{
    m_CurrentTime = 0.0;
    m_CurrentAnimation = animation;

    m_FinalBoneMatrices.reserve(100);

    for (int i = 0; i < 100; i++)
        m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::PlayAnimation(Animation* pAnimation)
{
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(m_CurrentTime);
        nodeTransform = Bone->GetLocalTransform();
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices()
{
    return m_FinalBoneMatrices;
}


/*############################################################# MESH #############################################################*/

// constructor
DynamicMesh::DynamicMesh(vector<DynamicModelVertex> vertices, vector<unsigned int> indices, vector<DynamicModelTexture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

// destructor
DynamicMesh::~DynamicMesh()
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
void DynamicMesh::Draw()
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
        glUniform1i(glGetUniformLocation(dynamicShaderProgObj, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// initializes all the buffer objects/arrays
void DynamicMesh::setupMesh()
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(DynamicModelVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_NORMAL);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TEXTURE0);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_TANGENT);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_BITANGENT);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, Bitangent));
    // ids
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_BONE_ID);
    glVertexAttribIPointer(DOMAIN_ATTRIBUTE_BONE_ID, 4, GL_INT, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_BONE_WEIGHT);
    glVertexAttribPointer(DOMAIN_ATTRIBUTE_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(DynamicModelVertex), (void*)offsetof(DynamicModelVertex, m_Weights));
    glBindVertexArray(0);
}


/*############################################################# MODEL #############################################################*/

// constructor, expects a filepath to a 3D model.
DynamicModel::DynamicModel(string const& path, bool gamma) : gammaCorrection(gamma)
{
    LOG("------------------------------------------------------------------------------\n");
    LOG("Entry to function = %s\n", __FUNCTION__);
    
    loadModel(path);
   
    LOG("Exit from function = %s\n", __FUNCTION__);
    LOG("------------------------------------------------------------------------------\n");
}

//destructor
DynamicModel::~DynamicModel()
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
void DynamicModel::Draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->Draw();
}

std::map<string, BoneInfo>& DynamicModel::GetBoneInfoMap()
{ 
    return m_BoneInfoMap; 
}

int& DynamicModel::GetBoneCount() 
{ 
    return m_BoneCounter; 
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void DynamicModel::loadModel(string const& path)
{
    LOG("Entry to function = %s\n", __FUNCTION__);
    LOG("model file path = %s\n", path.c_str());

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace);
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
void DynamicModel::processNode(aiNode* node, const aiScene* scene)
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

void DynamicModel::SetVertexBoneDataToDefault(DynamicModelVertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}

DynamicMesh* DynamicModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<DynamicModelVertex> vertices;
    vector<unsigned int> indices;
    vector<DynamicModelTexture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        DynamicModelVertex vertex;
        vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        
        SetVertexBoneDataToDefault(vertex);
        vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
        vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

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

        glm::vec3 v0 = vertices[index0].Position;
        glm::vec3 v1 = vertices[index1].Position;
        glm::vec3 v2 = vertices[index2].Position;

        glm::vec2 uv0 = vertices[index0].TexCoords;
        glm::vec2 uv1 = vertices[index1].TexCoords;
        glm::vec2 uv2 = vertices[index2].TexCoords;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

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
        vector<DynamicModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<DynamicModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<DynamicModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<DynamicModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    LOG("mesh number = %zu\n", meshes.size());

    ExtractBoneWeightForVertices(vertices, mesh, scene);

    // return a mesh object created from the extracted mesh data
    return new DynamicMesh(vertices, indices, textures);
}

void DynamicModel::SetVertexBoneData(DynamicModelVertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void DynamicModel::ExtractBoneWeightForVertices(std::vector<DynamicModelVertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    auto& boneInfoMap = m_BoneInfoMap;
    int& boneCount = m_BoneCounter;

    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = boneCount;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCount;
            boneCount++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

unsigned int DynamicModel::TextureFromFile(const char* path, const string& directory, bool gamma)
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

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<DynamicModelTexture> DynamicModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<DynamicModelTexture> textures;
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
            DynamicModelTexture texture;
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

/*############################################################# ANIMATOR #############################################################*/

void loadDynamicModel(const char* path, DYNAMIC_MODEL* dynamicModel)
{
    dynamicModel->pModel = new DynamicModel(path, true);
	if (dynamicModel->pModel == NULL)
	{
		LOG("loadDynamicModel() new DynamicModel Failed.\n");
	}

    dynamicModel->pAnimation = new Animation(path, dynamicModel->pModel);
    if (dynamicModel->pAnimation == NULL)
    {
        LOG("loadAnimatedModel() new Animation Failed.\n");
    }

    dynamicModel->pAnimator = new Animator(dynamicModel->pAnimation);
    if (dynamicModel->pAnimator == NULL)
    {
        LOG("loadAnimatedModel() new Animator Failed.\n");
    }
}

void drawDynamicModel(DYNAMIC_MODEL dynamicModel, float deltaTime)
{
    dynamicModel.pAnimator->UpdateAnimation(deltaTime);

    std::vector<glm::mat4> transforms = dynamicModel.pAnimator->GetFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++)
    {
        glUniformMatrix4fv(finalBonesMatricesUniform[i], 1, GL_FALSE, glm::value_ptr(transforms[i]));
    }

    dynamicModel.pModel->Draw(dynamicShaderProgObj);
}

void unloadDynamicModel(DYNAMIC_MODEL* dynamicModel)
{
	if (dynamicModel->pModel)
	{
		delete dynamicModel->pModel;
        dynamicModel->pModel = nullptr;
	}
}



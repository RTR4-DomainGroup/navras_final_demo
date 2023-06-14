#pragma once
#include<stdio.h>
#include "../../inc/helper/common.h"
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../ext/SOIL/include/SOIL.h"
#include "../../inc/helper/stb_image.h"
#include "../helper/assimp_glm_helpers.h"
#include "../helper/animdata.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define GLM_ENABLE_EXPERIMENTAL
#define MAX_BONE_INFLUENCE 4

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

struct DynamicModelVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct DynamicModelTexture {
    unsigned int id;
    string type;
    string path;
};

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class DynamicMesh {
public:

    // mesh Data
    vector<DynamicModelVertex>       vertices;
    vector<unsigned int> indices;
    vector<DynamicModelTexture>      textures;
    unsigned int VAO;

    // render data 
    unsigned int VBO, EBO;

    // constructor
    DynamicMesh(vector<DynamicModelVertex> vertices, vector<unsigned int> indices, vector<DynamicModelTexture> textures);

    // destructor
    ~DynamicMesh();

    // render the mesh
    void Draw();

    // initializes all the buffer objects/arrays
    void setupMesh();

};



class DynamicModel
{
public:

    // model data 
    vector<DynamicModelTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<DynamicMesh*>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    DynamicModel(string const& path, bool gamma = false);

    //destructor
    ~DynamicModel();

    // draws the model, and thus all its meshes
    void Draw();

    std::map<string, BoneInfo>& GetBoneInfoMap();

    int& GetBoneCount();

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    void SetVertexBoneDataToDefault(DynamicModelVertex& vertex);

    DynamicMesh* processMesh(aiMesh* mesh, const aiScene* scene);

    void SetVertexBoneData(DynamicModelVertex& vertex, int boneID, float weight);

    void ExtractBoneWeightForVertices(std::vector<DynamicModelVertex>& vertices, aiMesh* mesh, const aiScene* scene);

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<DynamicModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};


class Bone
{
public:

    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;
    int m_NumPositions;
    int m_NumRotations;
    int m_NumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    void Update(float animationTime);

    glm::mat4 GetLocalTransform();

    std::string GetBoneName() const;

    int GetBoneID();

    int GetPositionIndex(float animationTime);

    int GetRotationIndex(float animationTime);

    int GetScaleIndex(float animationTime);

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    glm::mat4 InterpolatePosition(float animationTime);

    glm::mat4 InterpolateRotation(float animationTime);

    glm::mat4 InterpolateScaling(float animationTime);
};

class Animation
{
public:

    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

    Animation() = default;

    Animation(const std::string& animationPath, DynamicModel* model);

    ~Animation();

    Bone* FindBone(const std::string& name);

    inline float GetTicksPerSecond();

    inline float GetDuration();

    inline const AssimpNodeData& GetRootNode();

    inline const std::map<std::string, BoneInfo>& GetBoneIDMap();

    void ReadMissingBones(const aiAnimation* animation, DynamicModel& model);

    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

};

class Animator
{
public:

    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;

    Animator(Animation* animation);

    void UpdateAnimation(float dt);

    void PlayAnimation(Animation* pAnimation);

    float GetCurrentFrame();

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

    std::vector<glm::mat4> GetFinalBoneMatrices();

};


//PROPERTIES OF VERTEX:
typedef struct _DYNAMIC_MODEL
{
    DynamicModel* pModel;
    Animation* pAnimation;
    Animator* pAnimator;

}DYNAMIC_MODEL;


void drawDynamicModel(ADSDynamicUniform adsDynamicUniform, DYNAMIC_MODEL dynamicModel, float deltaTime);
void loadDynamicModel(const char* path, DYNAMIC_MODEL* dynamicModel);
void unloadDynamicModel(DYNAMIC_MODEL* dynamicModel);


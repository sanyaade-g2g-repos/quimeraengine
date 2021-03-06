
#ifndef __QMODELLOADER__
#define __QMODELLOADER__


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "QStaticModelRawData.h"
#include "QModelAspectsRawData.h"

//#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING
#define QE_LOG_MODELLOAD(strMessage) QE_LOG(strMessage)
#else
#define QE_LOG_MODELLOAD(strMessage)
#endif

#undef max

// IMPORTANT: Float precision must match ASSIMP's floats precission

class QModelLoader
{
public:
    /*
    typedef QArrayDynamic<QKeyValuePair<QHashedString, QMaterial*>> LoadedMaterialArray;
    typedef QArrayDynamic<QKeyValuePair<QHashedString, QTexture2D*>> LoadedTextureArray;
    typedef QArrayDynamic<QKeyValuePair<QHashedString, QImage*>> LoadedImageArray;
    typedef QArrayDynamic<QKeyValuePair<QHashedString, QAspect*>> LoadedAspectArray;
    */

    // 1---------- Assign material indices to submeshes
    // 2------- Create Blender and Sampler
    // 3- Create textures from info
    // 4- Add aspects system to rendering

    // The vertex type should be flexible

    virtual void LoadModel(const QPath &filePath, const QVertexDescription* pVertexDescription, QStaticModelRawData &model, QModelAspectsRawData &modelAspects)
    {
        QE_LOG_MODELLOAD(string_q("Loading model from '") + filePath.GetAbsolutePath() + "'");

        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(filePath.ToString().ToBytes(EQTextEncoding::E_UTF8).Get(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_FindInvalidData | aiProcess_ValidateDataStructure | aiProcess_GenSmoothNormals | aiProcess_SortByPType);

        QE_ASSERT_ERROR(pScene != null_q && pScene->mFlags != AI_SCENE_FLAGS_INCOMPLETE && pScene->mRootNode != null_q,
                        string_q("An error occurred when loading a model from '") + filePath.ToString() + "'. " + importer.GetErrorString());

        QE_LOG_MODELLOAD("ASSIMP model loaded correctly\n");

        QE_LOG_MODELLOAD("Creating internal model...\n");

        QE_LOG_MODELLOAD("Processing ASSIMP materials...\n");

        QE_LOG_MODELLOAD("Loading internal textures...\n");
        
        // [TODO]: ProcessInternalTextures(pScene->mTextures, pScene->mNumTextures, modelAspects);
        
        QE_LOG_MODELLOAD("Internal textures loaded.\n");

        QE_LOG_MODELLOAD("Building model aspects...\n");

        QModelLoader::PrepareModelAspectsOutputData(pScene, modelAspects);

        QE_LOG_MODELLOAD("Creating aspects from ASSIMP materials...\n");

        ProcessMaterials(pScene->mMaterials, pScene->mNumMaterials, modelAspects);

        QE_LOG_MODELLOAD("Model aspects built.\n");

        QE_LOG_MODELLOAD("ASSIMP materials processed.\n");

        QModelLoader::PrepareStaticModelOutputData(pScene, pVertexDescription, model);
        

        QE_LOG_MODELLOAD("Processing ASSIMP model...\n");

        u32_q uMeshIndex = 1;
        u32_q uSubmeshIndex = 0;
        ProcessNode(pScene->mRootNode, model, 0, 0);
        //QE_ASSERT_ERROR(model.MeshIds[12].Id.GetLength() == 0, "ERROR");
        ProcessHierarchy(model, pScene->mMeshes, pScene->mRootNode, modelAspects, uMeshIndex, uSubmeshIndex, 0);

        QE_LOG_MODELLOAD("Freeing intermediate resources...\n");

        importer.FreeScene();

        QE_LOG_MODELLOAD("Model loaded.\n");
    }

protected:

    static void PrepareStaticModelOutputData(const aiScene* pScene, const QVertexDescription* pVertexDescription, QStaticModelRawData &model)
    {
        u32_q uNumVertices = 0;
        u32_q uNumIndices = 0;
        u32_q uNumNodes = CountNodes(pScene->mRootNode) + 1U;
        aiMesh** pCurrenMesh = pScene->mMeshes;

        for (u32_q i = 0; i < pScene->mNumMeshes; ++i, ++pCurrenMesh)
        {
            uNumVertices += (*pCurrenMesh)->mNumVertices;
            uNumIndices += (*pCurrenMesh)->mNumFaces * 3U;
        }

        model.VertexDescription = pVertexDescription;
        model.Vertices = new char[model.VertexDescription->GetVertexSize() * uNumVertices];
        model.VertexCount = uNumVertices;

        model.Indices = new u32_q[uNumIndices];
        model.IndexCount = uNumIndices;

        model.Meshes = new QStaticModelRawData::QMesh[uNumNodes];
        model.MeshIds = new QStaticModelRawData::QMeshId[uNumNodes];
        QE_ASSERT_ERROR(model.MeshIds[11].Id.GetLength() == 0, "ERROR");
        model.Submeshes = new QStaticModelRawData::QSubmesh[pScene->mNumMeshes];
        model.SubmeshAspects = new QStaticModelRawData::QSubmeshAspect[pScene->mNumMeshes];
        model.MeshCount = uNumNodes;
        model.SubmeshCount = pScene->mNumMeshes;

        QE_LOG_MODELLOAD(string_q("Vertex buffer: ") + string_q::FromInteger(uNumVertices) + " vertices\n");
        QE_LOG_MODELLOAD(string_q("Index buffer: ") + string_q::FromInteger(uNumIndices) + " indices\n");
        QE_LOG_MODELLOAD(string_q("Mesh buffer: ") + string_q::FromInteger(uNumNodes) + " meshes\n");
        QE_LOG_MODELLOAD(string_q("Submesh buffer: ") + string_q::FromInteger(pScene->mNumMeshes) + " submeshes\n");
        QE_LOG_MODELLOAD("\n");
    }

    static void PrepareModelAspectsOutputData(const aiScene* pScene, QModelAspectsRawData &modelAspects)
    {
        u32_q uNumExternalTextures = CountExternalTextures(pScene->mMaterials, pScene->mNumMaterials);
        u32_q uNumTextures = pScene->mNumTextures + uNumExternalTextures;

        modelAspects.AspectCount = pScene->mNumMaterials;
        modelAspects.Aspects = rcast_q(new char[pScene->mNumMaterials * sizeof(QAspect*)], QAspect**);
        modelAspects.AspectIds = new QHashedString[pScene->mNumMaterials];
        memset(modelAspects.Aspects, 0, sizeof(void*) * pScene->mNumMaterials);

        modelAspects.MaterialCount = pScene->mNumMaterials;
        modelAspects.Materials = rcast_q(new char[pScene->mNumMaterials * sizeof(QMaterial*)], QMaterial**);
        modelAspects.MaterialIds = new QHashedString[pScene->mNumMaterials];
        memset(modelAspects.Materials, 0, sizeof(void*) * pScene->mNumMaterials);

        modelAspects.TextureCount = uNumTextures;
        modelAspects.Textures = new QModelAspectsRawData::QTextureInfo[uNumTextures];
        modelAspects.TextureIds = new QHashedString[uNumTextures];
    }

    static u32_q CountExternalTextures(aiMaterial** arInputMaterials, const u32_q uMaterials)
    {
        QArrayDynamic<aiString, QPoolAllocator, SQEqualityComparator<aiString> > arCountedTextures;

        for (u32_q i = 0; i < uMaterials; ++i)
        {
            aiMaterial* pMaterial = arInputMaterials[i];
            ProcessTexturePath(aiTextureType_AMBIENT, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_DIFFUSE, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_SPECULAR, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_EMISSIVE, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_LIGHTMAP, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_DISPLACEMENT, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_REFLECTION, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_HEIGHT, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_OPACITY, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_NORMALS, pMaterial, arCountedTextures);
            ProcessTexturePath(aiTextureType_SHININESS, pMaterial, arCountedTextures);
            // NONE
            // UNKNOWN
        }

        return arCountedTextures.GetCount();
    }

    static void ProcessTexturePath(const aiTextureType textureType, const aiMaterial* pMaterial, QArrayDynamic<aiString, QPoolAllocator, SQEqualityComparator<aiString> > &arTexturePaths)
    {
        aiString texturePath;
        u32_q uTextures = pMaterial->GetTextureCount(textureType);

        for (u32_q i = 0; i < uTextures; ++i)
        {
            aiReturn result = pMaterial->Get(AI_MATKEY_TEXTURE(textureType, i), texturePath);
            
            QE_ASSERT_ERROR(result == aiReturn_SUCCESS, string_q("The path of the texture is missing: ") + textureType + " type, slot " + i);

            if (!arTexturePaths.Contains(texturePath))
                arTexturePaths.Add(texturePath);
        }
    }

    static void ProcessMaterials(aiMaterial** arInputMaterials, const u32_q uMaterials, QModelAspectsRawData &modelAspects)
    {

        // [TODO]: Optimize in such a way that existing elements (in the Resource manager) are not even loaded. Optional? A flag to choose whether they are not loaded or they are renamed or numerated?
        aiString materialName;
        u32_q uLastLoadedImage = -1;

        for (u32_q i = 0; i < uMaterials; ++i)
        {
            // ASPECT
            // ---------
            QAspect* pAspect = new QAspect();
            modelAspects.Aspects[i] = pAspect;

            if (arInputMaterials[i]->Get(AI_MATKEY_NAME, materialName) == aiReturn_SUCCESS)
                modelAspects.AspectIds[i] = materialName.C_Str();
            else
                modelAspects.AspectIds[i] = string_q("UNNAMED") + string_q::FromInteger(pAspect);

            i32_q nWireframeEnabled = 0;

            if (arInputMaterials[i]->Get(AI_MATKEY_ENABLE_WIREFRAME, nWireframeEnabled) == aiReturn_SUCCESS)
                pAspect->SetWireframeMode(true);

            i32_q nTwosided = 0;

            if (arInputMaterials[i]->Get(AI_MATKEY_TWOSIDED, nTwosided) == aiReturn_SUCCESS)
                pAspect->SetFaceCulling(false);

            aiBlendMode nBlendFunction = aiBlendMode_Default;

            if (arInputMaterials[i]->Get(AI_MATKEY_BLEND_FUNC, nBlendFunction) == aiReturn_SUCCESS)
                pAspect->SetAlphaBlendingMode(QModelLoader::GetEquivalentBlendingModeValue(nBlendFunction));

            f32_q fBlendOpacity = 0;

            if (arInputMaterials[i]->Get(AI_MATKEY_OPACITY, fBlendOpacity) == aiReturn_SUCCESS)
                pAspect->SetAlphaBlendingOpacity(fBlendOpacity);

            // MATERIAL
            // ----------
            QMaterial* pNewMaterial = new QMaterial();
            modelAspects.Materials[i] = pNewMaterial;
            modelAspects.MaterialIds[i] = modelAspects.AspectIds[i];
            ProcessMaterialColors(arInputMaterials[i], pNewMaterial, modelAspects);

            float_q fShininess = SQFloat::_0;

            if (arInputMaterials[i]->Get(AI_MATKEY_SHININESS, fShininess) == aiReturn_SUCCESS) // Is this specular?
                pNewMaterial->Shininess = fShininess;

            float_q fShininessStrength = SQFloat::_0;

            if (arInputMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, fShininess) == aiReturn_SUCCESS)
                pNewMaterial->SpecularPower = fShininessStrength;

            pAspect->SetMaterial(modelAspects.MaterialIds[i]);

            // IMAGES & TEXTURES
            // --------------------
            ProcessMaterialTextures(arInputMaterials[i], modelAspects, modelAspects.AspectIds[i], pAspect, uLastLoadedImage);
        }
    }

    static QAspect::EQAlphaBlendingMode GetEquivalentBlendingModeValue(const aiBlendMode blendMode)
    {
        QAspect::EQAlphaBlendingMode eResult = QAspect::E_Alpha;

        switch (blendMode)
        {
        case aiBlendMode_Default:
            eResult = QAspect::E_Alpha;
            break;
        case aiBlendMode_Additive:
            eResult = QAspect::E_Addition;
            break;
        }

        return eResult;
    }

    static void ProcessMaterialColors(const aiMaterial* pInputMaterial, QMaterial* pOutputMaterial, QModelAspectsRawData &modelAspects)
    {
        // This array must coincide in order with the QMaterial structure
        static const char* COLOR_COMPONENTS[] = { AI_MATKEY_COLOR_AMBIENT,
                                                  AI_MATKEY_COLOR_DIFFUSE,
                                                  AI_MATKEY_COLOR_EMISSIVE,
                                                  AI_MATKEY_COLOR_SPECULAR,
                                                  AI_MATKEY_COLOR_TRANSPARENT,
                                                  AI_MATKEY_COLOR_REFLECTIVE
                                                };

        aiVector3D color;
        QColor* pOutputColor = &pOutputMaterial->Ambient;

        for (u32_q i = 0; i < sizeof(COLOR_COMPONENTS) / sizeof(char*); i += 3) // +3 because the array stores (string, 0, 0)
        {
            if (pInputMaterial->Get(COLOR_COMPONENTS[i], 0, 0, color) == aiReturn_SUCCESS)
            {
                pOutputColor->x = color.x;
                pOutputColor->y = color.y;
                pOutputColor->z = color.z;
                pOutputColor->w = SQFloat::_1;

                ++pOutputMaterial;
            }
        }
    }

    static void ProcessMaterialTextures(aiMaterial* pInputMaterial, QModelAspectsRawData &modelAspects, const QHashedString aspectId, QAspect* pAspect, u32_q &uLastLoadedImage)
    {
        static const QAspect::EQAspectComponentType COMPONENT_TYPES[] = {   QAspect::E_Ambient,
                                                                            QAspect::E_Diffuse,
                                                                            QAspect::E_Specular,
                                                                            QAspect::E_Normals,
                                                                            QAspect::E_Opacity,
                                                                            QAspect::E_Displacement,
                                                                            QAspect::E_Height,
                                                                            QAspect::E_Shininess,
                                                                            QAspect::E_Reflection,
                                                                            QAspect::E_Lightmap,
                                                                            QAspect::E_Emissive
                                                                        };

        static const aiTextureType TEXTURE_TYPES[] = {  aiTextureType_AMBIENT,
                                                        aiTextureType_DIFFUSE,
                                                        aiTextureType_SPECULAR,
                                                        aiTextureType_NORMALS,
                                                        aiTextureType_OPACITY,
                                                        aiTextureType_DISPLACEMENT,
                                                        aiTextureType_HEIGHT,
                                                        aiTextureType_SHININESS,
                                                        aiTextureType_REFLECTION,
                                                        aiTextureType_LIGHTMAP,
                                                        aiTextureType_EMISSIVE
                                                     };
                                                    // NONE
                                                    // UNKNOWN

        for (u32_q i = 0; i < sizeof(COMPONENT_TYPES) / sizeof(COMPONENT_TYPES[0]); ++i)
            ProcessMaterialTexture(TEXTURE_TYPES[i], pInputMaterial, aspectId, COMPONENT_TYPES[i], pAspect, modelAspects, uLastLoadedImage);
    }

    static void ProcessMaterialTexture(const aiTextureType textureType, 
                                       const aiMaterial* pMaterial, 
                                       const QHashedString &aspectId, 
                                       const QAspect::EQAspectComponentType componentType,
                                       QAspect* pAspect, 
                                       QModelAspectsRawData &modelAspects,
                                       u32_q &uLastLoadedImage)
    {
        aiString texturePath;
        u32_q uTextures = pMaterial->GetTextureCount(textureType);
        string_q strTexturePath;

        for (u32_q i = 0; i < uTextures; ++i)
        {
            aiReturn result = pMaterial->Get(AI_MATKEY_TEXTURE(textureType, i), texturePath);

            QE_ASSERT_ERROR(result == aiReturn_SUCCESS, string_q("The path of the texture is missing: ") + textureType + " type, slot " + i);

            strTexturePath = texturePath.C_Str();
            if (strTexturePath[0] == '/' || strTexturePath[0] == '\'')
                strTexturePath = string_q(".") + strTexturePath;

            u32_q uExistingImageIndex = QModelLoader::GetIndexOfImageByPath(strTexturePath, modelAspects.Textures, uLastLoadedImage + 1U);

            if (uExistingImageIndex == -1)
            {
                ++uLastLoadedImage;

                // TEXTURE
                // -------------
                modelAspects.TextureIds[uLastLoadedImage] = QPath(texturePath.C_Str()).GetFilename();

                aiTextureMapping textureMapping = aiTextureMapping_OTHER;
                aiTextureMapMode textureMapMode[] = { aiTextureMapMode_Wrap, aiTextureMapMode_Wrap, aiTextureMapMode_Wrap };
                u32_q uUVindex = -1;
                f32_q fBlendFactor = SQFloat::_0;
                aiTextureOp textureOp = aiTextureOp_Add;

                if (pMaterial->GetTexture(textureType, i, &texturePath, &textureMapping, &uUVindex, &fBlendFactor, &textureOp, textureMapMode) == aiReturn_SUCCESS)
                {
                    modelAspects.Textures[uLastLoadedImage].BlendFactor = fBlendFactor;
                    modelAspects.Textures[uLastLoadedImage].BlendOperation = GetEquivalentBlendOperationValue(textureOp);
                    modelAspects.Textures[uLastLoadedImage].Mapping = GetEquivalentMappingValue(textureMapping);
                    modelAspects.Textures[uLastLoadedImage].TextureCoordsIndex = uUVindex;
                    modelAspects.Textures[uLastLoadedImage].WrapMode[0] = GetEquivalentWrapModeValue(textureMapMode[0]);
                    modelAspects.Textures[uLastLoadedImage].WrapMode[1] = GetEquivalentWrapModeValue(textureMapMode[1]);
                    modelAspects.Textures[uLastLoadedImage].WrapMode[2] = GetEquivalentWrapModeValue(textureMapMode[2]);
                    
                    modelAspects.Textures[uLastLoadedImage].FilePath = strTexturePath;

                    // [TODO]: Load UV transformations
                    aiUVTransform uvTransform;
                    pMaterial->Get(AI_MATKEY_UVTRANSFORM(textureType, i), uvTransform);
                }
                
                uExistingImageIndex = uLastLoadedImage;
            }
 
            pAspect->AddTextureLayer(componentType, modelAspects.TextureIds[uExistingImageIndex], modelAspects.TextureIds[uExistingImageIndex], modelAspects.TextureIds[uExistingImageIndex]);
        }
    }

    static QTexture2D::EQTextureMapping GetEquivalentMappingValue(const aiTextureMapping mapping)
    {
        QTexture2D::EQTextureMapping eResult = QTexture2D::E_UV;

        switch (mapping)
        {
        case aiTextureMapping_UV:
            eResult = QTexture2D::E_UV;
            break;
        case aiTextureMapping_BOX:
            eResult = QTexture2D::E_Cubic;
            break;
        case aiTextureMapping_SPHERE:
            eResult = QTexture2D::E_Spherical;
            break;
        case aiTextureMapping_CYLINDER:
            eResult = QTexture2D::E_Cylindrical;
            break;
        case aiTextureMapping_PLANE:
            eResult = QTexture2D::E_Planar;
            break;
        }

        return eResult;
    }

    static QTextureBlender::EQTextureBlendOperation GetEquivalentBlendOperationValue(const aiTextureOp blendOperation)
    {
        QTextureBlender::EQTextureBlendOperation eResult = QTextureBlender::E_Addition;

        switch (blendOperation)
        {
        case aiTextureOp_Add:
            eResult = QTextureBlender::E_Addition;
            break;
        case aiTextureOp_Subtract:
            eResult = QTextureBlender::E_Substraction;
            break;
        case aiTextureOp_Multiply:
            eResult = QTextureBlender::E_Mutiplication;
            break;
        case aiTextureOp_Divide:
            eResult = QTextureBlender::E_Division;
            break;
        case aiTextureOp_SmoothAdd:
            eResult = QTextureBlender::E_SmoothAddition;
            break;
        case aiTextureOp_SignedAdd:
            eResult = QTextureBlender::E_SignedAddition;
            break;
        }

        return eResult;
    }

    static QSampler2D::EQTextureWrapMode GetEquivalentWrapModeValue(const aiTextureMapMode wrapMode)
    {
        QSampler2D::EQTextureWrapMode eResult = QSampler2D::E_Border;

        switch (wrapMode)
        {
        case aiTextureMapMode_Clamp:
            eResult = QSampler2D::E_Clamp;
            break;
        case aiTextureMapMode_Decal:
            eResult = QSampler2D::E_Border;
            break;
        case aiTextureMapMode_Wrap:
            eResult = QSampler2D::E_Repeat;
            break;
        case aiTextureMapMode_Mirror:
            eResult = QSampler2D::E_RepeatMirror;
            break;
        }

        return eResult;
    }

    static u32_q GetIndexOfImageByPath(const string_q &imagePath, const QModelAspectsRawData::QTextureInfo* const arTextures, const u32_q uTextureCount)
    {
        u32_q i = 0;

        while (i < uTextureCount && arTextures[i].FilePath != imagePath)
            ++i;

        return i < uTextureCount ? i : -1;
    }

    static u32_q CountNodes(const aiNode* pNode)
    {
        u32_q uNumNodes = 0;

        for (u32_q i = 0; i < pNode->mNumChildren; ++i)
            uNumNodes += CountNodes(pNode->mChildren[i]);

        return pNode->mNumChildren + uNumNodes;
    }

    static void ProcessHierarchy(QStaticModelRawData &model, aiMesh** const arMeshes, const aiNode* pNode, const QModelAspectsRawData& modelAspects, u32_q &uOutpuMeshIndex, u32_q &uOutpuSubmeshIndex, const u32_q uParentMeshIndex)
    {
        //        A
        //       / \
        //      B   D
        //     /   / \
        //    C   E   F

        // Meshes    [  A   |   B  |  C   |  D   |  E   |   F  |      |      |      ]
        // Submeshes [  A1  |  A2  |  B1  |  C1  |  D1  |  D2  |  E1  |  F1  |      ]

        // The intent is that meshes in the same level under a node, or brother nodes, are contiguous

        QStaticModelRawData::QMesh* pParentMesh = model.Meshes + uParentMeshIndex;
        
        pParentMesh->FirstChildMesh = pNode->mNumChildren != 0 ? uOutpuMeshIndex : 
                                                                 -1;

        QE_LOG_MODELLOAD(string_q("Processing children of mesh[") + string_q::FromInteger(uParentMeshIndex) + "]...\n");
        QE_LOG_MODELLOAD(string_q("First child: ") + string_q::FromInteger(pParentMesh->FirstChildMesh) + "\n");

        QStaticModelRawData::QMesh* pPreviousMesh = null_q;

        for (u32_q i = 0; i < pNode->mNumChildren; ++i, ++uOutpuMeshIndex)
        {
            ProcessNode(pNode->mChildren[i], model, uOutpuMeshIndex, uOutpuSubmeshIndex);

            model.Meshes[uOutpuSubmeshIndex].SiblingMesh = -1;

            model.Meshes[uOutpuSubmeshIndex].SiblingMesh = i == pNode->mNumChildren ? -1 :
                                                                                      uOutpuSubmeshIndex + 1U;

            if (pNode->mChildren[i]->mNumMeshes > 0)
                ProcessMesh(arMeshes, pNode->mChildren[i]->mMeshes, model, uOutpuMeshIndex, modelAspects, uOutpuSubmeshIndex);
        }

        QE_LOG_MODELLOAD(string_q("Children of mesh[") + string_q::FromInteger(uParentMeshIndex) + "] processed.\n");
        QE_LOG_MODELLOAD("Processing every child's children...\n");

        for (u32_q i = 0; i < pNode->mNumChildren; ++i)
            ProcessHierarchy(model, arMeshes, pNode->mChildren[i], modelAspects, uOutpuMeshIndex, uOutpuSubmeshIndex, uParentMeshIndex + i);

        QE_LOG_MODELLOAD("Child's children processed.\n");
    }

    static void ProcessNode(aiNode* const pInputNode, QStaticModelRawData &model, const u32_q uMeshIndex, const u32_q uSubmeshIndex)
    {
        QE_LOG_MODELLOAD(string_q("Processing mesh[") + string_q::FromInteger(uMeshIndex) + "]...\n");

        QStaticModelRawData::QMesh* pMesh = model.Meshes + uMeshIndex;
        QStaticModelRawData::QMeshId* pMeshId = model.MeshIds + uMeshIndex;
        pMesh->SubmeshCount = pInputNode->mNumMeshes;
        pMesh->FirstSubmesh = uSubmeshIndex;
        pMeshId->Mesh = uMeshIndex;
        pMeshId->Id = pInputNode->mName.C_Str();
        QTransformationMatrix<QMatrix4x4> transformation(QMatrix4x4(&pInputNode->mTransformation.a1));
        transformation.Decompose(pMesh->Translation, pMesh->Rotation, pMesh->Scale);
        /*
        QE_LOG_MODELLOAD(string_q("First submesh: ") + string_q::FromInteger(pMesh->FirstSubmesh) + "\n");
        QE_LOG_MODELLOAD(string_q("Number of submeshes: ") + string_q::FromInteger(pMesh->SubmeshCount) + "\n");
        QE_LOG_MODELLOAD(string_q("Name: ") + pMeshId->Id + "\n");
        QE_LOG_MODELLOAD(string_q("Translation: ") + pMesh->Translation.ToString() + "\n");
        QE_LOG_MODELLOAD(string_q("Rotation: ") + pMesh->Rotation.ToString() + "\n");
        QE_LOG_MODELLOAD(string_q("Scale: ") + pMesh->Scale.ToString() + "\n");
        QE_LOG_MODELLOAD("\n");*/
    }

    static void ProcessMesh(aiMesh** const arExternalSubmeshes, unsigned int* arSubmeshes, QStaticModelRawData &model, const u32_q uMeshIndex, const QModelAspectsRawData& modelAspects, u32_q &uOutpuSubmeshIndex)
    {
        // [TODO]: Support instancing
        u32_q uNumSubmeshes = model.Meshes[uMeshIndex].SubmeshCount;

        for (u32_q i = 0; i < uNumSubmeshes; ++i, ++uOutpuSubmeshIndex)
        {
            ProcessSubmesh(arExternalSubmeshes[arSubmeshes[i]], model, uOutpuSubmeshIndex/*model.GetMeshByIndex(uMeshIndex)->FirstSubmesh + i*/);
            model.SubmeshAspects[uOutpuSubmeshIndex].Submesh = uOutpuSubmeshIndex;
            model.SubmeshAspects[uOutpuSubmeshIndex].AspectId = modelAspects.AspectIds[arExternalSubmeshes[arSubmeshes[i]]->mMaterialIndex];

            QE_LOG_MODELLOAD(string_q("Mesh[") + uMeshIndex + string_q("].Submesh[") + i + "] (actual submesh[" + uOutpuSubmeshIndex + "]), AspectId: " + model.SubmeshAspects[uOutpuSubmeshIndex].AspectId + "\n");
        }
    }

    static void ProcessSubmesh(const aiMesh* pExternalSubmesh, QStaticModelRawData &model, const u32_q uSubmeshIndex)
    {
        QE_LOG_MODELLOAD(string_q("Processing submesh[") + string_q::FromInteger(uSubmeshIndex) + "]...\n");

        QStaticModelRawData::QSubmesh* pSubmesh = model.Submeshes + uSubmeshIndex;
        pSubmesh->VertexCount = pExternalSubmesh->mNumVertices;
        pSubmesh->IndexCount = pExternalSubmesh->mNumFaces * 3U;
        pSubmesh->FirstVertex = 0;
        pSubmesh->FirstIndex = 0;
        pSubmesh->PrimitiveType = pExternalSubmesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE ? EQPrimitiveType::E_Triangle :
                                  pExternalSubmesh->mPrimitiveTypes & aiPrimitiveType_POLYGON  ? EQPrimitiveType::E_Polygon :
                                  pExternalSubmesh->mPrimitiveTypes & aiPrimitiveType_LINE     ? EQPrimitiveType::E_Line :
                                  pExternalSubmesh->mPrimitiveTypes & aiPrimitiveType_POINT    ? EQPrimitiveType::E_Point :
                                                                                                 EQPrimitiveType::E_Triangle;
        
        if (uSubmeshIndex != 0)
        {
            QStaticModelRawData::QSubmesh* pPreviousSubmesh = model.Submeshes + uSubmeshIndex - 1U;
            pSubmesh->FirstVertex = pPreviousSubmesh->FirstVertex + pPreviousSubmesh->VertexCount;
            pSubmesh->FirstIndex = pPreviousSubmesh->FirstIndex + pPreviousSubmesh->IndexCount;
        }
        /*
        QE_LOG_MODELLOAD(string_q("Number of vertices: ") + string_q::FromInteger(pSubmesh->VertexCount) + "\n");
        QE_LOG_MODELLOAD(string_q("Number of indices: ") + string_q::FromInteger(pSubmesh->IndexCount) + "\n");
        QE_LOG_MODELLOAD(string_q("First vertex: ") + string_q::FromInteger(pSubmesh->FirstVertex) + "\n");
        QE_LOG_MODELLOAD(string_q("First index: ") + string_q::FromInteger(pSubmesh->FirstIndex) + "\n");
        */

        float_q* pFirstValueInSubmesh = rcast_q(rcast_q(model.Vertices, pointer_uint_q) + pSubmesh->FirstVertex * model.VertexDescription->GetVertexSize(), float_q*);

        u32_q uComponentOffset = 0;
        u32_q uTextureCoordArrayIndex = 0;
        u32_q uColorArrayIndex = 0;
        const u32_q NUMBER_OF_VALUES_IN_VERTEX = model.VertexDescription->GetVertexSize() / sizeof(float_q);

        for (u32_q i = 0; i < model.VertexDescription->GetVertexComponentCount(); ++i)
        {
            switch (model.VertexDescription->GetVertexComponents()[i].Type)
            {
            case QVertexComponent::E_Position3D:
                Copy3DVectors(&pExternalSubmesh->mVertices->x, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                break;
            case QVertexComponent::E_Position4D:
                CopyVertexPositions4D(&pExternalSubmesh->mVertices->x, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                break;
            case QVertexComponent::E_Normal:
                if (pExternalSubmesh->mNormals)
                    Copy3DVectors(&pExternalSubmesh->mNormals->x, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                break;
            case QVertexComponent::E_TextureCoord2D:
                if (pExternalSubmesh->mTextureCoords && uTextureCoordArrayIndex < pExternalSubmesh->GetNumUVChannels())
                    CopyTextureCoords2D(&pExternalSubmesh->mTextureCoords[uTextureCoordArrayIndex]->x, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                ++uTextureCoordArrayIndex;
                break;
            case QVertexComponent::E_TextureCoord3D:
                if (pExternalSubmesh->mTextureCoords && uTextureCoordArrayIndex < pExternalSubmesh->GetNumUVChannels())
                    Copy3DVectors(&pExternalSubmesh->mTextureCoords[uTextureCoordArrayIndex]->x, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                ++uTextureCoordArrayIndex;
                break;
            case QVertexComponent::E_Color:
                if (pExternalSubmesh->mColors)
                    Copy4DVectors(&pExternalSubmesh->mColors[uColorArrayIndex]->r, pFirstValueInSubmesh + uComponentOffset, pSubmesh->VertexCount, NUMBER_OF_VALUES_IN_VERTEX);
                ++uColorArrayIndex;
                break;
            }

            uComponentOffset += model.VertexDescription->GetVertexComponents()[i].Size / sizeof(float_q);
        }

        CopyTriangleIndices(model.Indices + pSubmesh->FirstIndex, pExternalSubmesh->mFaces, pSubmesh->IndexCount);
    }

    static void CopyTriangleIndices(u32_q* arOutputIndices, const aiFace* arInputFaces, const u32_q uIndexCount)
    {
        for (u32_q i = 0; i < uIndexCount; ++i)
            arOutputIndices[i] = arInputFaces[i / 3U].mIndices[i % 3U];
        /*
        for (u32_q i = 0; i < uIndexCount / 3U; i += 3U)
        {
            arOutputIndices[i + 0] = arInputFaces[i].mIndices[2] + uIndexOffset;
            arOutputIndices[i + 1] = arInputFaces[i].mIndices[1] + uIndexOffset;
            arOutputIndices[i + 2] = arInputFaces[i].mIndices[0] + uIndexOffset;
        }*/
    }

    static void Copy2DVectors(float* pInput, float_q* pOutput, const u32_q uVectors, const u32_q uVertexSize)
    {
        for (u32_q i = 0; i < uVectors; ++i)
        {
            u32_q uOutputVectorComponentIndex = uVertexSize * i;
            u32_q uInputVectorComponentIndex = 2U * i;
            pOutput[0 + uOutputVectorComponentIndex] = pInput[0 + uInputVectorComponentIndex];
            pOutput[1 + uOutputVectorComponentIndex] = pInput[1 + uInputVectorComponentIndex];
        }
    }

    static void Copy3DVectors(float* pInput, float_q* pOutput, const u32_q uVectors, const u32_q uVertexSize)
    {
        for (u32_q i = 0; i < uVectors; ++i)
        {
            u32_q uOutputVectorComponentIndex = uVertexSize * i;
            u32_q uInputVectorComponentIndex = 3U * i;
            pOutput[0 + uOutputVectorComponentIndex] = pInput[0 + uInputVectorComponentIndex];
            pOutput[1 + uOutputVectorComponentIndex] = pInput[1 + uInputVectorComponentIndex];
            pOutput[2 + uOutputVectorComponentIndex] = pInput[2 + uInputVectorComponentIndex];
        }
    }

    static void Copy4DVectors(float* pInput, float_q* pOutput, const u32_q uVectors, const u32_q uVertexSize)
    {
        for (u32_q i = 0; i < uVectors; ++i)
        {
            u32_q uOutputVectorComponentIndex = uVertexSize * i;
            u32_q uInputVectorComponentIndex = 4U * i;
            pOutput[0 + uOutputVectorComponentIndex] = pInput[0 + uInputVectorComponentIndex];
            pOutput[1 + uOutputVectorComponentIndex] = pInput[1 + uInputVectorComponentIndex];
            pOutput[2 + uOutputVectorComponentIndex] = pInput[2 + uInputVectorComponentIndex];
            pOutput[3 + uOutputVectorComponentIndex] = pInput[3 + uInputVectorComponentIndex];
        }
    }

    static void CopyVertexPositions4D(float* pInputPosition, float_q* pOutputPosition, const u32_q uVertices, const u32_q uVertexSize)
    {
        for (u32_q i = 0; i < uVertices; ++i)
        {
            u32_q uOutputPositionComponentIndex = uVertexSize * i;
            u32_q uInputPositionComponentIndex = 3U * i;
            pOutputPosition[0 + uOutputPositionComponentIndex] = pInputPosition[0 + uInputPositionComponentIndex];
            pOutputPosition[1 + uOutputPositionComponentIndex] = pInputPosition[1 + uInputPositionComponentIndex];
            pOutputPosition[2 + uOutputPositionComponentIndex] = pInputPosition[2 + uInputPositionComponentIndex];
            pOutputPosition[3 + uOutputPositionComponentIndex] = SQFloat::_1;
        }
    }

    static void CopyTextureCoords2D(float* pInput, float_q* pOutput, const u32_q uVectors, const u32_q uVertexSize)
    {
        for (u32_q i = 0; i < uVectors; ++i)
        {
            u32_q uOutputVectorComponentIndex = uVertexSize * i;
            u32_q uInputVectorComponentIndex = 3U * i;
            pOutput[0 + uOutputVectorComponentIndex] = pInput[0 + uInputVectorComponentIndex];
            pOutput[1 + uOutputVectorComponentIndex] = pInput[1 + uInputVectorComponentIndex];
        }
    }

};

#endif // __QMODELLOADER__
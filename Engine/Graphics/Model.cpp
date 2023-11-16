#include "Model.h"

#include "Core/Graphics.h"
#include "Core/CommandContext.h"
#include "Core/SamplerManager.h"
#include "Renderer.h"
#include "DefaultTextures.h"

void Model::Create(const ModelData& modelData) {

    struct MaterialData {
        Vector3 diffuse;
        float pad;
        Vector3 specular;
    };

    std::vector<std::shared_ptr<TextureResource>> createdTextures;
    std::vector<std::shared_ptr<Material>> createdMaterials;

    CommandContext commandContext;
    commandContext.Create();
    // テクスチャ
    for (auto& srcTexture : modelData.textures) {
        auto& destTexture = createdTextures.emplace_back(std::make_shared<TextureResource>());

        if (!srcTexture.filePath.empty()) {
            destTexture->CreateFromWICFile(commandContext, srcTexture.filePath.wstring());
        }
    }
    // マテリアル
    for (auto& srcMaterial : modelData.materials) {
        auto& destMaterial = createdMaterials.emplace_back(std::make_shared<Material>());
        destMaterial->constantBuffer.CreateConstantBuffer(L"Model ConstantBuffer", sizeof(MaterialData));
        MaterialData materialData{};
        materialData.diffuse = srcMaterial.diffuse;
        materialData.specular = srcMaterial.specular;
        destMaterial->constantBuffer.Copy(materialData);

        if (srcMaterial.textureIndex < createdTextures.size()) {
            destMaterial->texture = createdTextures[srcMaterial.textureIndex];
        }
    }
    // メッシュ
    for (auto& srcMesh : modelData.meshes) {
        // 生成するメッシュ
        auto& destMesh = meshes_.emplace_back();
        // 頂点バッファを作成
        destMesh.vertexBuffer.Create(L"Model VertexBuffer", sizeof(srcMesh.vertices[0]), srcMesh.vertices.size());
        destMesh.vertexBuffer.Copy(srcMesh.vertices.data(), destMesh.vertexBuffer.GetBufferSize());
        // インデックスバッファを作成
        destMesh.indexCount = uint32_t(srcMesh.indices.size());
        destMesh.indexBuffer.Create(L"Model indexBuffer", sizeof(srcMesh.indices[0]), srcMesh.indices.size());
        destMesh.indexBuffer.Copy(srcMesh.indices.data(), destMesh.indexBuffer.GetBufferSize());

        assert(srcMesh.materialIndex < createdMaterials.size());
        destMesh.material = createdMaterials[srcMesh.materialIndex];
        assert(destMesh.material);
    }

    commandContext.Close();
    auto& commandQueue = Graphics::GetInstance()->GetCommandQueue();
    commandQueue.Excute(commandContext);
    commandQueue.Signal();
    commandQueue.WaitForGPU();
}


std::list<ModelInstance*> ModelInstance::instanceLists_;

ModelInstance::ModelInstance() {
    instanceLists_.emplace_back(this);
}

ModelInstance::~ModelInstance() {
    auto iter = std::find(instanceLists_.begin(), instanceLists_.end(), this);
    if (iter != instanceLists_.end()) {
        instanceLists_.erase(iter);
    }
}

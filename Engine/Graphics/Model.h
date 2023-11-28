#pragma once

#include <memory>
#include <vector>

#include "Core/GPUBuffer.h"
#include "Core/UploadBuffer.h"
#include "Core/TextureResource.h"

#include "ModelLoader.h"

class Model {
    friend class ModelRenderer;
public:
    using Vertex = ModelData::Vertex;
    using Index = ModelData::Index;

    void Create(const ModelData& modelData);
    
private:
    struct Material {
        UploadBuffer constantBuffer;
        std::shared_ptr<TextureResource> texture;
    };
    struct Mesh {
        UploadBuffer vertexBuffer;
        UploadBuffer indexBuffer;
        uint32_t indexCount{};
        std::shared_ptr<Material> material;
    };

    std::vector<Mesh> meshes_;
};

class ModelInstance {
    friend class ModelRenderer;
public:
    static const std::list<ModelInstance*>& GetInstanceList() { return instanceLists_; }

    ModelInstance();
    virtual ~ModelInstance();

    void SetModel(const std::shared_ptr<Model>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& worldMatrix) { worldMatrix_ = worldMatrix; }
    void SetUseLighting(bool useLighting) { useLighting_ = useLighting; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }
    void SetColor(const Vector3& color) { color_ = color; }
    void SetUseRimLight(bool useRimLight) { useRimLight_ = useRimLight; }
    void SetRimLightColor(const Vector3& color) { rimLightColor_ = color; }
    void SetreceiveShadow(bool receiveShadow) { receiveShadow_ = receiveShadow; }

    bool IsActive() const { return isActive_; }

private:
    static std::list<ModelInstance*> instanceLists_;

    ModelInstance(const ModelInstance&) = delete;
    ModelInstance& operator=(const ModelInstance&) = delete;
    ModelInstance(ModelInstance&&) = delete;
    ModelInstance& operator=(ModelInstance&&) = delete;

    std::shared_ptr<Model> model_;
    Matrix4x4 worldMatrix_;
    Vector3 color_ = Vector3::one;
    Vector3 rimLightColor_ = Vector3::one;
    bool useLighting_ = true;
    bool useRimLight_ = false;
    bool receiveShadow_ = false;
    bool isActive_ = true;
};
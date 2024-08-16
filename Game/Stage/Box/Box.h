#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class Box : public GameObject {
public:
    ~Box() {}

    void Initialize(bool drawModel = false);
    void Update();

    void SetTexture(const std::shared_ptr<Texture>& texture);

    void DrawImGui();

private:
    std::unique_ptr<ModelInstance> model_;
    std::unique_ptr<BoxCollider> collider_;

    // 回転保管用
    Vector3 rotate_;
    Vector3 color_ = Vector3::one;
};
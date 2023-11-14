#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"
#include "Collision/Collider.h"

class Box : public GameObject {
public:
    ~Box() {}

    void Initialize(const Vector3& position, const Vector4& rotate, const Vector3& scale);
    void Update();

    void DrawImGui();

private:
    std::unique_ptr<ToonModelInstance> model_;
    std::unique_ptr<BoxCollider> collider_;

    // 回転保管用
    Vector3 rotate_;

};
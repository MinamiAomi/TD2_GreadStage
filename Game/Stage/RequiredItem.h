#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

// 必須拾得物
class RequiredItem : public GameObject {
public:
    ~RequiredItem() {}

    void Initialize();
    void Update();

    void DrawImGui();

    bool GetIsAlived() { return isAlive_; }

private:
    std::unique_ptr<ModelInstance> model_;
    std::unique_ptr<BoxCollider> collider_;

    // 回転保管用
    Vector3 rotate_;
    Vector3 color_ = Vector3(1.0f, 0.0f, 0.0f);
    bool isAlive_ = true;
private:
    void OnCollision(const CollisionInfo& collisionInfo);
};
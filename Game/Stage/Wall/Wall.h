#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class Wall : public GameObject {
public:
    ~Wall() {}

    void Initialize(const Vector3& basePosition, float radian);
    void Update();

protected:
    std::unique_ptr<ModelInstance> model_;

    std::unique_ptr<BoxCollider> collider_;
    std::unique_ptr<BoxCollider> colliderWallRange_;

    Vector3 basePosition_;
};
#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"

class Floor : public GameObject {
public:
    ~Floor() {}

    void Initialize(const Vector3& basePosition, float radian);
    void Update();

protected:
    std::unique_ptr<ModelInstance> model_;

    std::unique_ptr<BoxCollider> collider_;

    Vector3 basePosition_;
};
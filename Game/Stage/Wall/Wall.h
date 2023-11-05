#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"
#include "Collision/Collider.h"

class Wall : public GameObject {
public:
    ~Wall() {}

    void Initialize(const Vector3& basePosition, float radian);
    void Update();

protected:
    std::unique_ptr<ToonModelInstance> model_;

    std::unique_ptr<BoxCollider> collider_;

    Vector3 basePosition_;
};
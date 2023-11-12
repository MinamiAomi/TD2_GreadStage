#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"
#include "Collision/Collider.h"

class Box : public GameObject {
public:
    ~Box() {}

    void Initialize(const Vector3& basePosition, float radian);
    void Update();

    void DrawImGui();

protected:
    std::unique_ptr<ToonModelInstance> model_;

    std::unique_ptr<BoxCollider> collider_;
    
};
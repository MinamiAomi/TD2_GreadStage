#pragma once
#include "Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/ToonModel.h"
#include "Collision/Collider.h"

class Ground : public GameObject {
public:
    ~Ground() {}

    void Initialize();
    void Update();

protected:
    std::unique_ptr<ToonModelInstance> model_;

    //std::unique_ptr<BoxCollider> collider_;

};


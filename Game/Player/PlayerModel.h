#pragma once

#include "Math/Transform.h"
#include "Graphics/Model.h"

class PlayerModel {
public:
    enum Part {
        kBody,
        kHead,
        kRightArm,
        kLeftArm,
        kRightFoot,
        kLeftFoot,
        kBag,

        kNumParts
    };
    
    void Initialize(Transform* transform);
    void Update();

private:
    const Transform* baseTransform_;

    Transform partTransform_[kNumParts];
    std::shared_ptr<ModelInstance> partModels_[kNumParts];
};
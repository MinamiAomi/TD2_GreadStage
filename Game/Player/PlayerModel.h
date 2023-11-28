#pragma once

#include <functional>
#include <optional>
#include <array>

#include "Math/Transform.h"
#include "Math/Animation.h"
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

    enum AnimationType {
        kWalk,

        kNumAnimationTypes
    };

    void Initialize(Transform* transform);
    void Update();

    void PlayAnimation(AnimationType animation, bool isLoop);
    void StopAnimation();

private:
    static const Vector3 kInitialTranslates[kNumParts];
    static std::function<void(PlayerModel&)> kAnimationTable[kNumAnimationTypes];
    static std::array<Animation::QuaternionNode, kNumParts> kWalkRotateAnimationTable;

    void WalkAnimation();
    // 続行する場合true
    bool UpdateAnimationParameter(float delta);

    const Transform* baseTransform_;

    Transform partTransform_[kNumParts];
    std::shared_ptr<ModelInstance> partModels_[kNumParts];

    float animationParameter_;
    std::optional<AnimationType> animation_;
    bool isLoop_;
};
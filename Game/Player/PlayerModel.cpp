#include "PlayerModel.h"

#include "Graphics/ResourceManager.h"

const Vector3 PlayerModel::kInitialTranslates[PlayerModel::kNumParts] = {
    {  0.0f,  1.0f,  0.0f },
    {  0.0f,  0.2f,  0.0f },
    { -0.2f,  0.05f, 0.0f },
    {  0.2f,  0.05f, 0.0f },

    { -0.2f, -0.43f, 0.0f },
    {  0.2f, -0.43f, 0.0f },
    {  0.0f,  0.0f, 0.0f },
};

std::function<void(PlayerModel&)> PlayerModel::kAnimationStepFuncTable[PlayerModel::kNumAnimationTypes] = {
    &PlayerModel::WaitAnimationStep,
    &PlayerModel::WalkAnimationStep,
    &PlayerModel::JumpAnimationStep,
    &PlayerModel::LandingAnimationStep,
    &PlayerModel::BlendAnimationStep,
};

std::function<PlayerModel::AnimationData(PlayerModel&, float)> PlayerModel::kAnimationTable[PlayerModel::kNumAnimationTypes] = {
    &PlayerModel::WaitAnimation,
    &PlayerModel::WalkAnimation,
    &PlayerModel::JumpAnimation,
    &PlayerModel::LandingAnimation,
    &PlayerModel::BlendAnimation,

};


Animation::FloatNode PlayerModel::kWalkHeightAnimation = { { { 0.0f, 0.0f }, { 0.1f, 0.5f }, { 0.0f, 1.0f } } };


std::array<Animation::QuaternionNode, PlayerModel::kNumParts> PlayerModel::kWalkRotateAnimationTable = {
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForYAxis(-5.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForYAxis(5.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
    Animation::QuaternionNode({ Quaternion::identity, 0.0f}),
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForXAxis(-60.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForXAxis(60.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForXAxis(60.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForXAxis(-60.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForXAxis(45.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForXAxis(-45.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForXAxis(-45.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForXAxis(45.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
    Animation::QuaternionNode({{Quaternion::identity, 0.0f}, {Quaternion::MakeForYAxis(-5.0f * Math::ToRadian), 0.25f}, {Quaternion::identity, 0.5f}, {Quaternion::MakeForYAxis(5.0f * Math::ToRadian), 0.75f}, {Quaternion::identity, 1.0f}}),
};

std::array<Animation::QuaternionNode, PlayerModel::kNumParts> PlayerModel::kJumpRotateAnimationTable = {
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(50.0f * Math::ToRadian), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::identity, 0.0f}),
};

std::array<Animation::QuaternionNode, PlayerModel::kNumParts> PlayerModel::kLandingRotateAnimationTable = {
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(-20.0f * Math::ToRadian), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForYAxis(-110.0f * Math::ToRadian) * Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForYAxis( 110.0f * Math::ToRadian) * Quaternion::MakeForXAxis(-Math::HalfPi), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(20.0f * Math::ToRadian) * Quaternion::MakeForZAxis(-30.0f * Math::ToRadian), 0.0f}),
    Animation::QuaternionNode({ Quaternion::MakeForXAxis(20.0f * Math::ToRadian) * Quaternion::MakeForZAxis( 30.0f * Math::ToRadian), 0.0f}),
    Animation::QuaternionNode({ Quaternion::identity, 0.0f}),

};

void PlayerModel::Initialize(Transform* transform) {
    baseTransform_ = transform;

    partTransform_[kBody].SetParent(baseTransform_);
    partTransform_[kHead].SetParent(&partTransform_[kBody]);
    partTransform_[kRightArm].SetParent(&partTransform_[kBody]);
    partTransform_[kLeftArm].SetParent(&partTransform_[kBody]);
    partTransform_[kRightFoot].SetParent(&partTransform_[kBody]);
    partTransform_[kLeftFoot].SetParent(&partTransform_[kBody]);
    partTransform_[kBag].SetParent(&partTransform_[kBody]);

    for (uint32_t i = 0; i < kNumParts; ++i) {
        partTransform_[i].translate = kInitialTranslates[i];
    }

    const char* kPartModelName[] = {
        "FrogBody",
        "FrogHead",
        "FrogRightArm",
        "FrogLeftArm",
        "FrogRightFoot",
        "FrogLeftFoot",
        "FrogBag",
    };
    auto rm = ResourceManager::GetInstance();
    for (uint32_t i = 0; i < kNumParts; ++i) {
        partModels_[i] = std::make_shared<ModelInstance>();
        partModels_[i]->SetModel(rm->FindModel(kPartModelName[i]));
    }
}

void PlayerModel::Update() {

    if (animation_) {
        AnimationType current = animation_.value();
        kAnimationStepFuncTable[current](*this);
        auto animationData = kAnimationTable[current](*this, animationParameter_);

        for (uint32_t i = 0; i < kNumParts; ++i) {
            partTransform_[i].scale = animationData[i].scale;
            partTransform_[i].rotate = animationData[i].rotate;
            partTransform_[i].translate = animationData[i].translate;
        }

        prevAnimationData_ = std::move(animationData);
    }

    for (uint32_t i = 0; i < kNumParts; ++i) {
        partTransform_[i].UpdateMatrix();
        partModels_[i]->SetWorldMatrix(partTransform_[i].worldMatrix);
    }
}

void PlayerModel::PlayAnimation(AnimationType animation, bool isLoop, bool isBlend, uint32_t blendAnimationLength) {
    // 前回のアニメーションが存在する
    // ブレンドからブレンドは不可
    if (animation_.has_value() && animation_ != kBlend && isBlend) {
        blendAnimationParameter_.prev = kAnimationTable[animation_.value()](*this, animationParameter_);
        blendAnimationParameter_.next = kAnimationTable[animation](*this, 0.0f);
        blendAnimationParameter_.nextAnimationType = animation;
        blendAnimationParameter_.length = blendAnimationLength;
        animation_ = kBlend;
    }
    else {
        animation_ = animation;
    }
    isLoop_ = isLoop;
    animationParameter_ = 0.0f;
}

void PlayerModel::StopAnimation() {
    animation_ = std::nullopt;
}

void PlayerModel::WaitAnimationStep() {

    static const uint32_t kCycle = 90;

    animationParameter_ += 1.0f / kCycle;
    if (animationParameter_ >= 1.0f) {
        if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            animation_ = std::nullopt;
            return;
        }
        animationParameter_ -= float(int(animationParameter_));
    }
}

void PlayerModel::WalkAnimationStep() {
    static const uint32_t kCycle = 90;

    animationParameter_ += 1.0f / kCycle;
    if (animationParameter_ >= 1.0f) {
        if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            animation_ = std::nullopt;
            return;
        }
        animationParameter_ -= float(int(animationParameter_));
    }
}

void PlayerModel::JumpAnimationStep() {
    static const uint32_t kCycle = 120;

    animationParameter_ += 1.0f / kCycle;
    if (animationParameter_ >= 1.0f) {
        if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            animation_ = std::nullopt;
            return;
        }
        animationParameter_ -= float(int(animationParameter_));
    }
}

void PlayerModel::LandingAnimationStep() {
    static const uint32_t kCycle = 30;

    animationParameter_ += 1.0f / kCycle;
    if (animationParameter_ >= 1.0f) {
        if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            PlayAnimation(kWait, true, true, 30);
            return;
        }
        animationParameter_ -= float(int(animationParameter_));
    }
}

void PlayerModel::BlendAnimationStep() {

    animationParameter_ += 1.0f / blendAnimationParameter_.length;
    if (animationParameter_ >= 1.0f) {
        animation_ = blendAnimationParameter_.nextAnimationType;
        animationParameter_ -= float(int(animationParameter_));
    }
}

PlayerModel::AnimationData PlayerModel::WaitAnimation(float animationTime) {
    AnimationData data;
    for (uint32_t i = 0; i < kNumParts; ++i) {
        data[i].translate = kInitialTranslates[i];
        data[i].rotate = Quaternion::identity;
        data[i].scale = Vector3::one;
    }
    (void)animationTime;
    return data;
}

PlayerModel::AnimationData PlayerModel::WalkAnimation(float animationTime) {
    AnimationData data;

    for (uint32_t i = 0; i < kNumParts; ++i) {
        data[i].translate = kInitialTranslates[i];
        data[i].rotate = kWalkRotateAnimationTable[i].GetInterpolatedValue(animationTime);
        data[i].scale = Vector3::one;
    }

    data[kBody].translate.y = kInitialTranslates[kBody].y - kWalkHeightAnimation.GetInterpolatedValue(animationTime);
    data[kLeftFoot].translate.y = kInitialTranslates[kLeftFoot].y + kWalkHeightAnimation.GetInterpolatedValue(animationTime);
    data[kRightFoot].translate.y = kInitialTranslates[kRightFoot].y + kWalkHeightAnimation.GetInterpolatedValue(animationTime);
    return data;
}

PlayerModel::AnimationData PlayerModel::JumpAnimation(float animationTime) {
    AnimationData data;
    for (uint32_t i = 0; i < kNumParts; ++i) {
        data[i].translate = kInitialTranslates[i];
        data[i].rotate = kJumpRotateAnimationTable[i].GetInterpolatedValue(animationTime);
        data[i].scale = Vector3::one;
    }
    data[kBody].rotate = Quaternion::MakeForXAxis(-Math::HalfPi);

    (void)animationTime;
    return data;
}

PlayerModel::AnimationData PlayerModel::LandingAnimation(float animationTime) {
    AnimationData data;
    for (uint32_t i = 0; i < kNumParts; ++i) {
        data[i].translate = kInitialTranslates[i];
        data[i].rotate = kLandingRotateAnimationTable[i].GetInterpolatedValue(animationTime);
        data[i].scale = Vector3::one;
    }
   // data[kBody].rotate = Quaternion::MakeForXAxis(-Math::HalfPi);

    (void)animationTime;
    return data;
}

PlayerModel::AnimationData PlayerModel::BlendAnimation(float animationTime) {

    AnimationData data;
    
    for (uint32_t i = 0; i < kNumParts; ++i) {
        data[i].translate = Vector3::Lerp(animationTime, blendAnimationParameter_.prev[i].translate, blendAnimationParameter_.next[i].translate);
        data[i].rotate = Quaternion::Slerp(animationTime, blendAnimationParameter_.prev[i].rotate, blendAnimationParameter_.next[i].rotate);
        data[i].scale = Vector3::Lerp(animationTime, blendAnimationParameter_.prev[i].scale, blendAnimationParameter_.next[i].scale);
    }
    return data;
}

bool PlayerModel::UpdateAnimationParameter(float delta) {
    animationParameter_ += delta;
    if (animationParameter_ >= 1.0f) {
        if (animation_ == kBlend) {
            animation_ = blendAnimationParameter_.nextAnimationType;
        }
        else if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            animation_ = std::nullopt;
            return false;
        }
        animationParameter_ -= float(int(animationParameter_));
    }
    return true;
}

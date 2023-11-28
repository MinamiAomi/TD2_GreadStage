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

std::function<void(PlayerModel&)> PlayerModel::kAnimationTable[PlayerModel::kNumAnimations] = {
    &PlayerModel::WalkAnimation,
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
        kAnimationTable[animation_.value()](*this);
    }
    for (uint32_t i = 0; i < kNumParts; ++i) {
        partTransform_[i].UpdateMatrix();
        partModels_[i]->SetWorldMatrix(partTransform_[i].worldMatrix);
    }
}

void PlayerModel::PlayAnimation(Animation animation, bool isLoop) {
    animation_ = animation;
    isLoop_ = isLoop;
    animationParameter_ = 0.0f;
}

void PlayerModel::StopAnimation() {
    animation_ = std::nullopt;
}

void PlayerModel::WalkAnimation() {
    static const uint32_t kAnimationCycle = 120;

    if (!UpdateAnimationParameter(1.0f / kAnimationCycle)) {
        return;
    }


    // 足アニメーション
    {


        partTransform_[kLeftFoot];
        partTransform_[kRightFoot];
    }
}

bool PlayerModel::UpdateAnimationParameter(float delta) {
    animationParameter_ += delta;
    if (animationParameter_ >= 1.0f) {
        if (!isLoop_) {
            // 終了
            animationParameter_ = 1.0f;
            animation_ = std::nullopt;
            return false;
        }
        animationParameter_ = float(int(animationParameter_));
    }
    return true;
}

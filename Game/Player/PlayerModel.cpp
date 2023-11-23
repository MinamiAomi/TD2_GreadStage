#include "PlayerModel.h"
#include "Graphics/ResourceManager.h"

void PlayerModel::Initialize(Transform* transform) {
    baseTransform_ = transform;

    partTransform_[kBody].SetParent(baseTransform_);
    partTransform_[kHead].SetParent(&partTransform_[kBody]);
    partTransform_[kRightArm].SetParent(&partTransform_[kBody]);
    partTransform_[kLeftArm].SetParent(&partTransform_[kBody]);
    partTransform_[kRightFoot].SetParent(&partTransform_[kBody]);
    partTransform_[kLeftFoot].SetParent(&partTransform_[kBody]);
    partTransform_[kBag].SetParent(&partTransform_[kBody]);

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



    for (uint32_t i = 0; i < kNumParts; ++i) {
        partTransform_[i].UpdateMatrix();
        partModels_[i]->SetWorldMatrix(partTransform_[i].worldMatrix);
    }
}

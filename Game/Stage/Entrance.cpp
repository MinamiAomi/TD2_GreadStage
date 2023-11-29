#include "Entrance.h"

#include <cassert>
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

#include "Input/Input.h"
#include "Transition/Transition.h"

void Entrance::Initialize(const int& number) {
    assert(number > 0 && number <= 6);

    std::string numStr = std::to_string(number);

    SetName("Entrance" + numStr);
    wellModel_ = std::make_unique<ModelInstance>();
    boardModel_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    transform.scale = Vector3::one;

    wellModel_->SetModel(ResourceManager::GetInstance()->FindModel("StartWell"));
    wellModel_->SetIsActive(true);
    wellModel_->SetColor(color_);
    wellModel_->SetReceiveShadow(false);

    boardModel_->SetModel(ResourceManager::GetInstance()->FindModel("StartWellBoard"));
    boardModel_->SetIsActive(true);
    boardModel_->SetColor(color_);
    boardModel_->SetCoverTexture(ResourceManager::GetInstance()->FindTexture("Board" + numStr));
    boardModel_->SetReceiveShadow(false);

    collider_->SetName("Entrance" + numStr);
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    collider_->SetCollisionAttribute(CollisionConfig::Entrance);
    collider_->SetCollisionMask(CollisionConfig::Player);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

    stageNumber_ = number;
    isAlive_ = true;
    animationType_ = AnimationType::Normal;
}

void Entrance::Update() {
    AnimationUpdate();

    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(Vector3{ 0.0f, 0.5f, 0.0f} * transform.worldMatrix);
    wellModel_->SetWorldMatrix(transform.worldMatrix);
    boardModel_->SetWorldMatrix(transform.worldMatrix);
}

void Entrance::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Player") {
        Transition::GetInstance()->SetComeToStage(stageNumber_);
    }
}

void Entrance::AnimationUpdate() {
    
}

void Entrance::DrawImGui() {
#ifdef _DEBUG
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
#endif // DEBUG
}

#include "Entrance.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

#include "Input/Input.h"
#include "Transition/Transition.h"

void Entrance::Initialize(const int& number) {
    SetName("Entrance" + std::to_string(number));
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Box"));
    model_->SetIsActive(true);
    model_->SetColor(color_);

    collider_->SetName("Entrance" + std::to_string(number));
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    collider_->SetCollisionAttribute(CollisionConfig::Stage);
    collider_->SetCollisionMask(~CollisionConfig::Stage);
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
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void Entrance::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Player") {
        if (Input::GetInstance()->IsKeyTrigger(DIK_M)) {
            Transition::GetInstance()->SetComeToStage(stageNumber_);
        }
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

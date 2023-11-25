#include "Entrance.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

void Entrance::Initialize(const int& number) {
    SetName("Entrance");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Star"));
    model_->SetIsActive(true);
    model_->SetColor(color_);

    collider_->SetName("Entrance");
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
        
    }
}

void Entrance::AnimationUpdate() {
    
}

void Entrance::DrawImGui() {

}

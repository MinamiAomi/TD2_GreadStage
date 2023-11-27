#include "RequiredItem.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

void RequiredItem::Initialize() {
    SetName("RequiredItem");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Moon"));
    model_->SetIsActive(true);
    model_->SetColor(color_);

    collider_->SetName("RequiredItem");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    collider_->SetCollisionAttribute(CollisionConfig::Stage);
    collider_->SetCollisionMask(~CollisionConfig::Stage);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

    isAlive_ = true;
    animationType_ = AnimationType::Normal;

}

void RequiredItem::Update() {
    AnimationUpdate();

    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void RequiredItem::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Player") {
        animationType_ = AnimationType::Get;
    }
}

void RequiredItem::AnimationUpdate() {
    switch (animationType_)
    {
    case AnimationType::Normal:
        TypeNormalUpdate();
        break;
    case AnimationType::Get:
        TypeGetUpdate();
        break;
    }
}

void RequiredItem::TypeNormalUpdate() {

}

void RequiredItem::TypeGetUpdate() {
    isAlive_ = false;
}

void RequiredItem::DrawImGui() {
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
    if (ImGui::ColorEdit3("color", &color_.x)) {
        model_->SetColor(color_);
    }
}

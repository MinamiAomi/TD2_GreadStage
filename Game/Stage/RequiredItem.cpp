#include "RequiredItem.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

#include "Audio/Audio.h"

void RequiredItem::Initialize() {
    SetName("RequiredItem");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Moon"));
    model_->SetIsActive(true);
    model_->SetColor(color_);
    model_->SetReceiveShadow(false);

    collider_->SetName("RequiredItem");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(Vector3::Scale(transform.scale, { 3.0f,3.0f,1.0f }));
    collider_->SetCenter(transform.translate);
    collider_->SetCollisionAttribute(CollisionConfig::Item);
    collider_->SetCollisionMask(CollisionConfig::Player);
    collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

    soundHandle_ = ResourceManager::GetInstance()->FindSound("MoonGet");

    isAlive_ = true;
    animationType_ = AnimationType::Normal;

}

void RequiredItem::Update() {
    AnimationUpdate();

    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void RequiredItem::OnCollision(const CollisionInfo& collisionInfo) {
    if (animationType_ != AnimationType::Get) {
        if (collisionInfo.collider->GetName() == "Player") {
            animationType_ = AnimationType::Get;
            playHandle_ = Audio::GetInstance()->SoundPlayWave(soundHandle_);
            Audio::GetInstance()->SetValume(playHandle_, 1.2f);
        }
    }
}

void RequiredItem::AnimationUpdate() {
    switch (animationType_) {
    case AnimationType::Normal:
        TypeNormalUpdate();
        break;
    case AnimationType::Get:
        TypeGetUpdate();
        break;
    }
}

void RequiredItem::TypeNormalUpdate() {
    transform.rotate = Quaternion::MakeFromAngleAxis(1.0f * Math::ToRadian, transform.rotate.GetUp()) * transform.rotate;

}

void RequiredItem::TypeGetUpdate() {
    isAlive_ = false;
}

void RequiredItem::DrawImGui() {
#ifdef _DEBUG

    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
    if (ImGui::ColorEdit3("color", &color_.x)) {
        model_->SetColor(color_);
    }
#endif // _DEBUG
}

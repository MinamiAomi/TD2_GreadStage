#include "Box.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"

void Box::Initialize(bool drawModel) {
    SetName("Box");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Box"));
    model_->SetIsActive(drawModel);

    //rotate_ = Vector3(rotate.x, rotate.y, rotate.z) * Math::ToRadian;

    collider_->SetName("Box");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    collider_->SetCollisionAttribute(CollisionConfig::Stage);
    collider_->SetCollisionMask(~CollisionConfig::Stage);
}

void Box::Update() {
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void Box::SetTexture(const std::shared_ptr<Texture>& texture) {
    model_->SetCoverTexture(texture);
}

void Box::DrawImGui() {
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

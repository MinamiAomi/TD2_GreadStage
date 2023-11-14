#include "Box.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void Box::Initialize(const Vector3& position, const Vector4& rotate, const Vector3& scale) {
    SetName("Box");
    model_ = std::make_unique<ToonModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Box"));
    model_->SetIsActive(true);
    model_->SetUseOutline(false);

    rotate_ = Vector3(rotate.x, rotate.y, rotate.z);

    transform.translate = position;
    transform.rotate = Quaternion::MakeFromAngleAxis(rotate.w, Vector3(rotate.x, rotate.y, rotate.z));
    transform.scale = scale;
    transform.UpdateMatrix();

    collider_->SetName("Box");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
}

void Box::Update() {
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void Box::DrawImGui() {
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
}

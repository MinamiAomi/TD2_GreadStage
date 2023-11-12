#include "Box.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void Box::Initialize(const Vector3& basePosition, float radian) {
    SetName("Box");
    model_ = std::make_unique<ToonModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Box"));
    model_->SetIsActive(true);
    model_->SetUseOutline(false);

    transform.translate = basePosition;
    transform.rotate = Quaternion::MakeForYAxis(radian);
    transform.scale = Vector3::one;
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
    static Vector3 r = { 0.0f, 0.0f,0.0f };
    ImGui::DragFloat3("rotate", &r.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(r * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
}

#include "TitleText.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

void TitleText::Initialize() {
    SetName("TitleText");
    model_ = std::make_unique<ModelInstance>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel(GetName()));
    model_->SetIsActive(true);
    model_->SetUseRimLight(false);
    model_->SetColor(Vector3::one);

    rotate_ = Vector3(0.0f,180.0f,0.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    transform.translate = Vector3(0.0f, 6.0f, -10.0f);

}

void TitleText::Update() {
    transform.UpdateMatrix();
    model_->SetWorldMatrix(transform.worldMatrix);
}

void TitleText::DrawImGui() {
#ifdef _DEBUG
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
#endif // DEBUG
}

void TitleText::AnimationUpdate() {

}

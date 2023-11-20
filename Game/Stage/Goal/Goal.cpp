#include "Goal.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"

Goal::~Goal() {

}

void Goal::Initialize() {
    SetName("Goal");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("Box"));
    model_->SetIsActive(true);

    collider_->SetName("Goal");
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetColor(Vector3::unitY);
}

void Goal::Update() {
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetWorldMatrix(transform.worldMatrix);
}

#include "Ground.h"

#include "Graphics/ResourceManager.h"


void Ground::Initialize() {
	SetName("Ground");
	model_ = std::make_unique<ToonModelInstance>();

	model_->SetModel(ResourceManager::GetInstance()->FindModel("Ground"));
	model_->SetIsActive(true);
	model_->SetUseOutline(false);

}

void Ground::Update() {
	transform.UpdateMatrix();
	model_->SetWorldMatrix(transform.worldMatrix);
}

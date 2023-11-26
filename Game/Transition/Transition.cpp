#include "Transition.h"
#include "Graphics/ResourceManager.h"

Transition* Transition::GetInstance() {
	static Transition instance;
	return &instance;
}

void Transition::Initialize() {
	texture_ = std::make_unique<Sprite>();
	texture_->SetTexture(ResourceManager::GetInstance()->FindTexture("Block"));
	texture_->SetIsActive(true);
	texture_->SetDrawOrder(1);
	texture_->SetScale(Vector2(1280.0f, 720.0f));
	texture_->SetPosition(Vector2(640.0f, 360.0f));
	texture_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

bool Transition::Update() {
	if (!isTransition_) {
		return false;
	}
	isTransition_ = false;
	return true;
}

void Transition::SetComeToStage(const int& stageNum) { 
	number = stageNum;
	isTransition_ = true;
}

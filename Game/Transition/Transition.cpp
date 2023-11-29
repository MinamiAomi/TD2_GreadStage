#include "Transition.h"
#include "Graphics/ResourceManager.h"

Transition* Transition::GetInstance() {
	static Transition instance;
	return &instance;
}

void Transition::Initialize() {
	number = 1;
	isTransition_ = true;
	scale_ = Vector2::zero;
	fadeSpeed_ = Vector2(20.0f, 20.0f);

	texture_ = std::make_unique<Sprite>();
	texture_->SetTexture(ResourceManager::GetInstance()->FindTexture("Block"));
	texture_->SetIsActive(true);
	texture_->SetDrawOrder(10);

	texture_->SetScale(scale_);
  
	texture_->SetPosition(Vector2(640.0f, 360.0f));
	texture_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

bool Transition::Update() {
	if (!isTransition_) {
		return false;
	}
	const Vector2 kWindowsize = Vector2(1280.0f, 720.0f);
	Vector2 volume = Vector2(kWindowsize.x / fadeSpeed_.x, kWindowsize.y / fadeSpeed_.y);

	if (fadeInOutFlag_) {
		scale_ += volume;
		texture_->SetScale(scale_);
		if (scale_.x >= kWindowsize.x && scale_.y >= kWindowsize.y) {
			fadeInOutFlag_ = false;
			return true;
		}
		return false;
	}
	else {
		scale_ -= volume;
		texture_->SetScale(scale_);
		if (scale_.x <= 0.0f && scale_.y <= 0.0f) {
			fadeInOutFlag_ = true;
			isTransition_ = false;
			scale_ = Vector2::zero;
			texture_->SetIsActive(false);
		}
		return false;
	}
}

void Transition::SetComeToStage(const int& stageNum) { 
	number = stageNum;
	isTransition_ = true;
	texture_->SetIsActive(true);
}

#include "Player.h"

#include "Graphics/ResourceManager.h"

void Player::Initialize() {
	SetName("Player");

	// 初期座標の設定
	transform.translate.z = -20.0f;

	// モデルの取得
	model_ = std::make_unique<ToonModelInstance>();
	model_->SetModel(ResourceManager::GetInstance()->FindModel("Player"));
	model_->SetIsActive(true);
}

void Player::Update() {
	
	UpdateTransform();
}

Vector3 Player::GetTranslate() const {
	Vector3 result;
	result.x = transform.worldMatrix.m[3][0];
	result.y = transform.worldMatrix.m[3][1];
	result.z = transform.worldMatrix.m[3][2];
	return result;
}

void Player::UpdateTransform() {
	transform.UpdateMatrix();
}

#include "Player.h"

#include "Graphics/ResourceManager.h"
#include "Engine/Input/Input.h"

void Player::Initialize() {
	SetName("Player");

	// 初期座標の設定
	transform.translate.z = -20.0f;

	// モデルの取得
	model_ = std::make_unique<ToonModelInstance>();
	model_->SetModel(ResourceManager::GetInstance()->FindModel("Player"));
	model_->SetIsActive(true);

	modelTrans_.translate = Vector3(0.0f, 0.8f, 0.0f);
	modelTrans_.UpdateMatrix();
	// 座標更新してからでなければローカルデータが消えてしまう
	modelTrans_.SetParent(&transform);

}

void Player::Update() {
	
	Move();
	MoveLimit();

	UpdateTransform();
}

void Player::UpdateTransform() {
	// 座標更新
	transform.UpdateMatrix();
	modelTrans_.UpdateMatrix();
	// モデル座標更新
	model_->SetWorldMatrix(modelTrans_.worldMatrix);
}

void Player::KeyInput() {
	

}

void Player::Move() {

	auto input = Input::GetInstance();
	Vector3 move;

	// キーボードでの移動
	if (input->IsKeyPressed(DIK_W)) {
		move.z += moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_S)) {
		move.z -= moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_A)) {
		move.x -= moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_D)) {
		move.x += moveSpeed_;
	}



	// 移動処理
	if (move != Vector3::zero) {
		move = move.Normalized();
		// カメラの角度に移動ベクトルを回転
		move = camera_->GetCamera()->GetRotate() * move;
		move = move.Normalized() * moveSpeed_;
		move.y = 0.0f;

		// 移動
		transform.translate += move;
		// 回転
		transform.rotate = Quaternion::Slerp(0.2f, transform.rotate, Quaternion::MakeLookRotation(move));
	}
}

void Player::MoveLimit() {
	if (transform.translate.y <= 0.0f) {
		transform.translate.y = 0.0f;
	}
}

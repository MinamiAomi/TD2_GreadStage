#include "Player.h"

#include "Graphics/ResourceManager.h"
#include "Engine/Input/Input.h"
#include "Collision/CollisionManager.h"

#include "Graphics/ImGuiManager.h"

void Player::Initialize() {
	SetName("Player");

	// 初期座標の設定
	transform.translate.z = -20.0f;

	// モデルの取得
	model_ = std::make_unique<ToonModelInstance>();
	model_->SetModel(ResourceManager::GetInstance()->FindModel("Player"));
	model_->SetIsActive(true);

	modelTrans_.translate = colliderOffset_;
	modelTrans_.UpdateMatrix();
	// 座標更新してからでなければローカルデータが消えてしまう
	modelTrans_.SetParent(&transform);

	collider_ = std::make_unique<BoxCollider>();
	collider_->SetGameObject(this);
	collider_->SetName("Player");
	collider_->SetCenter(modelTrans_.translate);
	collider_->SetSize({ 1.0f, 2.0f, 1.0f });
	collider_->SetCallback([this](const CollisionInfo& collisionInfo) { OnCollision(collisionInfo); });

	// パラメーター初期化
	moveSpeed_ = 0.5f;
	jumpParamerets_.isJumped_ = false;
	jumpParamerets_.fallSpeed_ = 0.0f;
	jumpParamerets_.jumpPower_ = 1.0f;

}

void Player::Update() {
	
	MoveUpdate();
	JumpUpdate();

	MoveLimit();

	isWallRun_ = false;

	UpdateTransform();
}

void Player::UpdateTransform() {
	// 座標更新
	transform.UpdateMatrix();
	modelTrans_.UpdateMatrix();

	Vector3 scale, translate;
	Quaternion rotate;
	transform.worldMatrix.GetAffineValue(scale, rotate, translate);
	collider_->SetCenter(translate + colliderOffset_);
	collider_->SetOrientation(rotate);

	// モデル座標更新
	model_->SetWorldMatrix(modelTrans_.worldMatrix);
}

void Player::MoveUpdate() {

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
		if (isWallRun_) {
			move.y += moveSpeed_;
		}
		else {
			move.x -= moveSpeed_;
		}
	}
	if (input->IsKeyPressed(DIK_D)) {
		if (isWallRun_) {
			move.y -= moveSpeed_;
		}
		else {
			move.x += moveSpeed_;
		}
	}

	// 移動処理
	if (move != Vector3::zero) {
		move = move.Normalized();
		// カメラの角度に移動ベクトルを回転
		move = camera_->GetCamera()->GetRotate() * move;
		move = move.Normalized() * moveSpeed_;
		// Y軸移動を削除
		if (!isWallRun_) {
			move.y = 0.0f;
		}

		// 移動
		transform.translate += move;
		// 回転
		transform.rotate = Quaternion::Quaternion::MakeLookRotation(move);
	}
}

void Player::MoveLimit() {
	jumpParamerets_.isJumped_ = true;
	if (transform.translate.y <= 0.0f) {
		jumpParamerets_.isJumped_ = false;
		transform.translate.y = 0.0f;
	}
}

void Player::JumpUpdate() {
	auto input = Input::GetInstance();
	const float gravity = 0.08f;

	if (!jumpParamerets_.isJumped_ && input->IsKeyTrigger(DIK_SPACE)) {
		jumpParamerets_.isJumped_ = true;
		jumpParamerets_.fallSpeed_ = jumpParamerets_.jumpPower_;
	}

	if (jumpParamerets_.isJumped_) {
		jumpParamerets_.fallSpeed_ -= gravity;
		transform.translate.y += jumpParamerets_.fallSpeed_;
	}


}

void Player::OnCollision(const CollisionInfo& collisionInfo) {
	if (collisionInfo.collider->GetName() == "Floor") {
		// ワールド空間の押し出しベクトル
		Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
		transform.translate += pushVector;

		// 衝突位置の法線
		float dot = Dot(collisionInfo.normal, Vector3::up);
		// 地面と見なす角度
		const float kGroundGradientAngle = 45.0f * Math::ToRadian;
		if (std::abs(std::acos(dot)) < kGroundGradientAngle) {
			jumpParamerets_.isJumped_ = false;
			jumpParamerets_.fallSpeed_ = 0.0f;
		}
		ImGui::Text("floorTrue");
	}

	if (collisionInfo.collider->GetName() == "Wall") {
		ImGui::Text("wallTrue");
		// ワールド空間の押し出しベクトル
		Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
		transform.translate += pushVector;

		// 壁の右側法線の位置との衝突位置の取得
		float dotRight = Dot(collisionInfo.normal, Vector3::right);
		// 壁の左側法線の位置との衝突位置の取得
		float dotLeft = Dot(collisionInfo.normal, Vector3::left);
		// 壁と見なす角度
		const float kWallDownAngle = 45.0f * Math::ToRadian;
		if (std::abs(std::acos(dotRight)) < kWallDownAngle
			|| std::abs(std::acos(dotLeft)) < kWallDownAngle) {
			isWallRun_ = true;
			jumpParamerets_.isJumped_ = false;
		}
	}
		
	
	UpdateTransform();

}

#include "Player.h"

#include "Graphics/ResourceManager.h"
#include "Engine/Input/Input.h"
#include "Collision/CollisionManager.h"

#include "Graphics/ImGuiManager.h"
#include <numbers>

void Player::Initialize() {
	SetName("Player");

	// 初期座標の設定
	transform.translate.z = -20.0f;
	//transform.rotate = Quaternion::MakeForYAxis(0.45f);

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
#ifdef _DEBUG
	DrawImGui();
	if (Input::GetInstance()->IsKeyTrigger(DIK_R)) {
		transform.translate = Vector3::zero;
	}
#endif // DEBUG

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
		move.x -= moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_D)) {
		move.x += moveSpeed_;
	}

	if (isWallRun_) {
		//move = move.Normalized();
		/*auto ro = Quaternion::Quaternion::MakeFromTwoVector(move, dotUp_);
		move = ro * move;*/

		//move = move - (Dot(move, dotUp_) * dotUp_);
		
		

		Vector3 characterDir = move.Normalized();
		Vector3 wallDir = dotUp_.Normalized();

		// キャラクターの進行ベクトルを壁に対して射影
		float dotProduct = Dot(characterDir, wallDir);
		if (dotProduct < 0.0f) {
			// 移動ベクトルを強引に上へ
			move = Vector3(0.0f, 0.5f, 0.0f);
		}
	}
	
	// 移動処理
	if (move != Vector3::zero) {
		move = move.Normalized();
		// カメラの角度に移動ベクトルを回転
		move = camera_->GetCamera()->GetRotate() * move;
		// Y軸移動を削除
		if (!isWallRun_) {
			move.y = 0.0f;
		}
		else {
			//move = Cross(move, dotUp_);
			/*float a = Vector3::Angle(move, dotUp_);
			float cosA = std::cos(a);
			float sinA = std::sin(a);
			move.x = move.x * cosA - move.y * sinA;
			move.y = move.x * sinA + move.y * cosA;*/
		}
		//move = transform.rotate * move.Normalized() * moveSpeed_;
		move = move.Normalized() * moveSpeed_;
		// 移動
		transform.translate += move;
		// 回転
		/*if (Dot(transform.rotate.GetForward(), move.Normalized()) < 0.9999f) {
			Quaternion diff = Quaternion::MakeFromTwoVector(transform.rotate.GetForward(), move.Normalized());
		}*/
		//transform.rotate = Quaternion::Slerp(0.1f, transform.rotate, Quaternion::MakeLookRotation(move));
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
	else {
		jumpParamerets_.fallSpeed_ = 0.0f;
	}


}

void Player::WallUpdate(Vector3 moveVec) {
	moveVec;
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
	}

	if (collisionInfo.collider->GetName() == "Wall") {
		// ワールド空間の押し出しベクトル
		Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
		transform.translate += pushVector;

		// 壁の右側法線の位置との衝突位置の取得
		float dotRight = Dot(collisionInfo.normal, Vector3::right);
		// 壁の左側法線の位置との衝突位置の取得
		float dotLeft = Dot(collisionInfo.normal, Vector3::left);

		dotUp_ = collisionInfo.normal;
		dotLeft_ = Cross(collisionInfo.normal, Vector3::left);

		// Quaternionは後ろからかける
		Vector3 up = transform.rotate.GetUp();
		Vector3 normal = collisionInfo.normal.Normalized();
		if (Dot(up, normal) < 0.9999f) {
			Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
			transform.rotate = diff * transform.rotate;
		}

		// 壁と見なす角度
		const float kWallDownAngle = 45.0f * Math::ToRadian;
		if (std::abs(std::acos(dotRight)) < kWallDownAngle
			|| std::abs(std::acos(dotLeft)) < kWallDownAngle) {
			
		}
		isWallRun_ = true;
		jumpParamerets_.isJumped_ = false;
	}
		
	
	UpdateTransform();

}

void Player::DrawImGui() {
	ImGui::DragFloat3("trans", &transform.translate.x, 0.1f);
}

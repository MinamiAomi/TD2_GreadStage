#include "Player.h"

#include "Graphics/ResourceManager.h"
#include "Engine/Input/Input.h"
#include "Collision/CollisionManager.h"

#include "Graphics/ImGuiManager.h"
#include <numbers>

void Player::Initialize() {
	SetName("Player");

	// 初期座標の設定
	transform.translate = Vector3(0.0f, 2.0f, -20.0f);
	//transform.rotate = Quaternion::MakeForYAxis(0.45f);

	// モデルの取得
	model_ = std::make_unique<ToonModelInstance>();
	model_->SetModel(ResourceManager::GetInstance()->FindModel("Player"));
	model_->SetIsActive(true);

	// 座標更新してからでなければローカルデータが消えてしまう
	modelTrans_.SetParent(&transform);
	modelTrans_.translate = {};
	modelTrans_.UpdateMatrix();

	collider_ = std::make_unique<BoxCollider>();
	collider_->SetGameObject(this);
	collider_->SetName("Player");
	collider_->SetCenter(transform.translate + colliderOffset_);
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
	Vector3 rotOffset = rotate * colliderOffset_;
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
		move.y += moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_S)) {
		move.y -= moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_A)) {
		move.x -= moveSpeed_;
	}
	if (input->IsKeyPressed(DIK_D)) {
		move.x += moveSpeed_;
	}

	auto xinput = input->GetXInputState();
	if (std::abs(xinput.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		move.x = 0.0f;
		move.x = xinput.Gamepad.sThumbLX;
	}
	if (std::abs(xinput.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		move.y = 0.0f;
		move.y = xinput.Gamepad.sThumbLY;
	}

	// 移動処理
	if (move != Vector3::zero) {
		move = move.Normalized();

		Matrix4x4 viewProjInv = camera_->GetCamera()->GetViewProjectionMatrix().Inverse();
		Vector3 origin = viewProjInv.ApplyTransformWDivide(Vector3::zero);
		move = viewProjInv.ApplyTransformWDivide(move);
		move = (move - origin).Normalized();

		Quaternion rotate = Quaternion::MakeFromTwoVector(-camera_->GetCamera()->GetForward(), transform.rotate.GetUp());
		move = rotate * move;

		move *= moveSpeed_;

		// 移動
		transform.translate += move;

	}
		
}

void Player::MoveLimit() {
	jumpParamerets_.isJumped_ = true;
	if (transform.translate.y <= -10.0f) {
		jumpParamerets_.isJumped_ = false;
		transform.translate = Vector3(0.0f, 2.0f, -20.0f);
		transform.rotate = Quaternion::identity;
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
			// Quaternionは後ろからかける
			Vector3 up = transform.rotate.GetUp();
			Vector3 normal = collisionInfo.normal.Normalized();
			if (Dot(up, normal) < 0.9999f) {
				Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
				transform.rotate = diff * transform.rotate;
			}
		}
	}

	if (collisionInfo.collider->GetName() == "Wall") {
		// ワールド空間の押し出しベクトル
		Vector3 pushVector = collisionInfo.normal * collisionInfo.depth;
		transform.translate += pushVector;

		// Quaternionは後ろからかける
		Vector3 up = transform.rotate.GetUp();
		Vector3 normal = collisionInfo.normal.Normalized();
		if (Dot(up, normal) < 0.9999f) {
			Quaternion diff = Quaternion::MakeFromTwoVector(up, normal);
			transform.rotate = diff * transform.rotate;
		}

		isWallRun_ = true;

	}

	if (collisionInfo.collider->GetName() == "WallRange") {

		jumpParamerets_.isJumped_ = false;
	}
		
	UpdateTransform();

}

void Player::DrawImGui() {
	ImGui::Begin("test", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Player")) {
			if (ImGui::TreeNode("Translate")) {
				ImGui::DragFloat3("trans", &transform.translate.x, 0.1f);
				ImGui::TreePop();
			}
			ImGui::Text("A");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Player2")) {
			if (ImGui::TreeNode("Rotation")) {
				ImGui::DragFloat3("rota", &transform.rotate.x, 0.1f);
				ImGui::TreePop();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

#pragma once
#include "Collision/GameObject.h"
#include "Graphics/ToonModel.h"

#include <memory>

#include "Math/Transform.h"
#include "CameraAnimation/CameraAnimation.h"
#include "Collision/Collider.h"

class Player : public GameObject {
public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();

public: // ゲッター
	Transform GetTransform() { return transform; }

public: // セッター
	void SetCamera(const std::shared_ptr<CameraAnimation>& camera) { camera_ = camera; }

private: // メンバ変数
	// モデル
	std::unique_ptr<ToonModelInstance> model_;
	std::shared_ptr<CameraAnimation> camera_;

	Transform modelTrans_;

	std::unique_ptr<BoxCollider> collider_;
	Vector3 colliderOffset_ = { 0.0f,0.5f,0.0f };

	float moveSpeed_ = 0.0f;

	struct JumpParameters {
		bool isJumped_ = false; /*true している false していない*/
		float fallSpeed_ = 0.0f;
		float jumpPower_ = 1.0f;
	};
	JumpParameters jumpParamerets_;
	
	bool isWallRun_ = false;

	Vector3 dotLeft_;
	Vector3 dotUp_;


private: // メンバ関数
	// 座標更新
	void UpdateTransform();
	// 移動
	void MoveUpdate();
	// 移動制限
	void MoveLimit();
	// ジャンプ
	void JumpUpdate();

	void WallUpdate(Vector3 moveVec);

	void OnCollision(const CollisionInfo& collisionInfo);


	void DrawImGui();
};
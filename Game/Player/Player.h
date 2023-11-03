#pragma once
#include "Collision/GameObject.h"
#include "Graphics/ToonModel.h"

#include <memory>

#include "Math/Transform.h"
#include "CameraAnimation/CameraAnimation.h"

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

	float moveSpeed_ = 1.0f;
	bool isMoved_; /*true 動いている false 止まっている*/

private: // メンバ関数
	// 座標更新
	void UpdateTransform();
	// キー入力
	void KeyInput();
	// 移動
	void Move();
	// 移動制限
	void MoveLimit();

};
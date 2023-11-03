#pragma once
#include "Collision/GameObject.h"
#include "Graphics/ToonModel.h"

#include <memory>

#include "Math/Transform.h"

class Player : public GameObject {
public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();

public: // ゲッター
	Vector3 GetTranslate() const;

private: // メンバ変数
	// モデル
	std::unique_ptr<ToonModelInstance> model_;

private: // メンバ関数
	// 座標更新
	void UpdateTransform();

};
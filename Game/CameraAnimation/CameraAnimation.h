#pragma once
#include "Math/Camera.h"
#include "Math/Transform.h"
#include <memory>

class CameraAnimation
{
public:
	void Initialize();
	void Update();
	void Restart();

public: // ゲッター
	const std::shared_ptr<Transform>& GetTransform() { return transform_; }
	const std::shared_ptr<Camera>& GetCamera() { return camera_; }

public: //	セッター
	void SetTarget(const Transform* target) { target_ = target; }

private: //	メンバ変数
	std::shared_ptr<Transform> transform_;
	std::shared_ptr<Camera> camera_;

	const Transform* target_ = nullptr;

};
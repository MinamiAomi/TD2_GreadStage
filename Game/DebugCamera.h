#pragma once
#include "Math/Camera.h"
#include "Math/Transform.h"
#include <memory>

class DebugCamera {
public:
	void Initialize();
	void Update();

	void SetCamera();

public: // ゲッター
	void SetRotate(const Quaternion& rotate) { eulerAngle_ = rotate.EulerAngle(); }
	const std::shared_ptr<Transform>& GetTransform() { return transform_; }
	const std::shared_ptr<Camera>& GetCamera() { return camera_; }

private: //	メンバ変数
	std::shared_ptr<Transform> transform_;
	std::shared_ptr<Camera> camera_;
	Vector3 eulerAngle_;
};

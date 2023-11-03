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

	const std::shared_ptr<Transform>& GetTransform() { return transform_; }
	const std::shared_ptr<Camera>& GetCamera() { return camera_; }

private:
	std::shared_ptr<Transform> transform_;
	std::shared_ptr<Camera> camera_;


};

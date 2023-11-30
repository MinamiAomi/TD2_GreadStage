#pragma once
#include "Engine/Collision/GameObject.h"

#include <memory>

#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Collision/Collider.h"
#include "Math/Random.h"

class Goal : public GameObject {
public:
	Goal() = default;
	~Goal() override = default;

	void Initialize();
	void Update(uint32_t currentItemCount);

	void DrawImGui();

private:
	std::unique_ptr<ModelInstance> model_;
	std::unique_ptr<BoxCollider> collider_;
	Vector3 rotate_;
	Random::RandomNumberGenerator ranGen_;
};
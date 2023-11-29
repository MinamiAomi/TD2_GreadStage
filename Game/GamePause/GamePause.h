#pragma once
#include "Graphics/Sprite.h"
#include <memory>
#include "Math/Color.h"
#include <vector>
#include <string>

class GamePause {
public:
	void Initialize();
	void Update();
	void SetDraw(const bool& flag);

private:
	std::vector<std::unique_ptr<Sprite>> texture_;
	const uint32_t kMaxTextures_ = 5;
	struct TextureParam {
		std::string name_;
		Vector2 scale_;
		float rotate_;
		Vector2 position_;
		uint8_t order_;
		Vector4 color_;
		bool isActive_;
	};
	TextureParam textureParam_[5];
	enum TextureName {
		StageSelect,
		Restart,
		Pose,
		Controller,
		BackGround,
	};

	bool isSelected_ = false; // true : ステージセレクトへ / false : リスタート
	bool preIsSelected_ = false;

	float easeTime_ = 0.0f;
	bool changeFlag_ = false;

private:
	void TextureUpdate();
	void SelectUpdate();

};

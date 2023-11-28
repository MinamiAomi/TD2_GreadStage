#pragma once
#include "Graphics/Sprite.h"
#include <memory>
#include "Math/Color.h"

class GamePause {
public:
	void Initialize();
	void Update();

private:
	std::unique_ptr<Sprite> stageSelect_;
	std::unique_ptr<Sprite> restart_;
	std::unique_ptr<Sprite> pose_;
	std::unique_ptr<Sprite> controller_;
	std::unique_ptr<Sprite> backGround_;

	struct TextureParam {
		Vector2 scale_;
		float rotate_;
		Vector2 position_;
		uint8_t order_;
		uint32_t color_;
		bool isActive_;
	};
	TextureParam stageSelectParam_;
	TextureParam restartParam_;
	TextureParam poseParam_;
	TextureParam controllerParam_;
	TextureParam backGroundParam_;

private:
	void TextureInitialize(Sprite* sprite, TextureParam param);

};

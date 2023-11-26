#pragma once
#include <string>
#include <memory>

#include "Graphics/Sprite.h"

class Transition {
public:
	static Transition* GetInstance();

	void Initialize();

	bool Update();

public: // セッター
	// どのステージに行くかのセット
	void SetComeToStage(const int& stageNum = 1);

public: // ゲッター
	std::string GetStageId() { return Directory + std::to_string(number); }

private:
	Transition() = default;
	~Transition() = default;

private:
	int number = 1;
	Vector2 fadeSpeed_;
	bool fadeInOutFlag_ = false; // In : true / Out : false
	bool isTransition_ = false;
	const std::string Directory = "Stage";
	std::unique_ptr<Sprite> texture_;
	Vector2 scale_;

};

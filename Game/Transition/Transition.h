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
	void SetComeToStage(const int& stageNum);

public: // ゲッター
	std::string GetStageId() { return Directory + std::to_string(number); }

private:
	Transition() = default;
	~Transition() = default;

private:
	int number = 1;
	bool isTransition_ = false;
	const std::string Directory = "Stage";
	std::unique_ptr<Sprite> texture_;

};

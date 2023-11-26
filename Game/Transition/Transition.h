#pragma once
#include <string>

class Transition {
public:
	static Transition* GetInstance();

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
	int number = 0;
	bool isTransition_ = false;
	const std::string Directory = "Stage";
};

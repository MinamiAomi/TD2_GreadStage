#pragma once

enum class TransitionType {
	Fade,
};

class Transition {
public:
	Transition() = default;
	~Transition() = default;

	static Transition* GetInstance();

	void SceneSet(TransitionType type);
	void Update();

private:
	// シーン切り替え用フラグ
	bool sceneChangeFlag_ = false;
	// トランジション開始のフラグ true = in/false = out
	bool inTransition_ = false;
	TransitionType type_;

};


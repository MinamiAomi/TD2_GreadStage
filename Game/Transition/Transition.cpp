#include "Transition.h"

Transition* Transition::GetInstance() {
	static Transition instance;
	return &instance;
}

void Transition::SceneSet(TransitionType type) {
	sceneChangeFlag_ = true;
	type_ = type;
}

void Transition::Update() {
	if (sceneChangeFlag_) {
		// フェードインの処理
		if (inTransition_) {

		}
		// フェードアウトの処理
		else {

		}
	}
}

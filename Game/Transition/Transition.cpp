#include "Transition.h"

Transition* Transition::GetInstance() {
	static Transition instance;
	return &instance;
}

bool Transition::Update() {
	if (!isTransition_) {
		return false;
	}
	isTransition_ = false;
	return true;
}

void Transition::SetComeToStage(const int& stageNum) { 
	number = stageNum;
	isTransition_ = true;
}

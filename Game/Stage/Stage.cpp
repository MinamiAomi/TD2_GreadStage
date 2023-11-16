#include "Stage.h"

#include "GlobalVariables/GlobalVariables.h"

void Stage::Initialize() {
}

void Stage::Update() {

	for (auto& i : boxes_) {
		i->Update();
	}
}

void Stage::Add(const std::shared_ptr<Box>& box) {
	boxes_.emplace_back(box);
}

void Stage::Load(const std::filesystem::path& loadFile) {
    GlobalVariables* global = GlobalVariables::GetInstance();
	std::string selectName = loadFile.string();
	global->LoadFile(selectName);
	global->LoadMessage(selectName);
	int num = global->GetIntValue(selectName, "Confirmation : ");
	boxes_.clear(); // 要素の全削除
	for (int i = 0; i < num; i++) {
		Vector3 trans = global->GetVector3Value(selectName, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str());
		Quaternion rot = global->GetQuaternionValue(selectName, ("BoxNumber : " + std::to_string(i) + " : Rotate").c_str());
		Vector3 scal = global->GetVector3Value(selectName, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str());
		auto& box = boxes_.emplace_back(std::make_shared<Box>());
		box->transform.translate = trans;
		box->transform.rotate = rot;
		box->transform.scale = scal;
		box->Initialize();
	}
}

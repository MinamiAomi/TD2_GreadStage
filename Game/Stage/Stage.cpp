#include "Stage.h"

#include "GlobalVariables/GlobalVariables.h"
#include <cassert>
#include "Graphics/RenderManager.h"

#include "Graphics/ImGuiManager.h"

void Stage::Initialize() {
	goal_ = std::make_shared<Goal>();
	goal_->Initialize();

	light_ = DirectionalLight::Create();
	light_->color = { 1.0f,1.0f,1.0f };

	starrySky_ = std::make_shared<StarrySky>();
	starrySky_->Initialize();
	RenderManager::GetInstance()->AddCustomRenderer(starrySky_);
	starrySky_->SetCamera(RenderManager::GetInstance()->GetCamera());
}

void Stage::Update() {

	for (auto& i : boxes_) {
		i->Update();
	}
	for (auto& i : items_) {
		i->Update();
	}
	for (auto& i : collects_) {
		i->Update();
	}
	for (auto& i : entrances_) {
		i->Update();
	}

	std::erase_if(items_, [](const std::shared_ptr<RequiredItem> item)
		{
			return !item->GetIsAlived();
		});

	std::erase_if(collects_, [](const std::shared_ptr<CollectionObject> collect)
		{
			return !collect->GetIsAlived();
		});

	goal_->Update();

	player_->SimpleUpdate();

	static float volume = 0.0f;
	static float knee = 0.0f;
	static float threshold = 0.0f;
	ImGui::DragFloat("volume", &volume, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("knee", &knee, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("threshold", &threshold, 0.01f, 0.0f, 1.0f);
	starrySky_->SetVolume(volume);
	RenderManager::GetInstance()->GetBloom().SetKnee(knee);
	RenderManager::GetInstance()->GetBloom().SetThreshold(threshold);

}

void Stage::Add(const std::shared_ptr<Box>& box) {
	boxes_.emplace_back(box)->Initialize();
}

void Stage::Add(const std::shared_ptr<RequiredItem>& item) {
	items_.emplace_back(item)->Initialize();
}

void Stage::Add(const std::shared_ptr<CollectionObject>& collect) {
	collects_.emplace_back(collect)->Initialize();
}

void Stage::Add(const std::shared_ptr<Entrance>& entrance) {
	static unsigned int num = 0;
	++num;
	assert(num <= kStageCount_);
	entrances_.emplace_back(entrance)->Initialize(num);
}

void Stage::DeleteBox(const int& num) {
	boxes_.erase(boxes_.begin() + num);
}

void Stage::DeleteItem(const int& num) {
	items_.erase(items_.begin() + num);
}

void Stage::DeleteCollect(const int& num) {
	collects_.erase(collects_.begin() + num);
}

void Stage::DeleteEntrance(const int& num) {
	entrances_.erase(entrances_.begin() + num);
}

void Stage::Load(const std::filesystem::path& loadFile) {
    GlobalVariables* global = GlobalVariables::GetInstance();
	std::string selectName = loadFile.string();
	global->LoadFile(selectName);
	
	int num = global->GetIntValue(selectName, "BoxConfirmation");
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
	
	num = global->GetIntValue(selectName, "ItemConfirmation");
	items_.clear(); // 要素の全削除
	for (int i = 0; i < num; i++) {
		Vector3 trans = global->GetVector3Value(selectName, ("ItemNumber : " + std::to_string(i) + " : Translate").c_str());
		Quaternion rot = global->GetQuaternionValue(selectName, ("ItemNumber : " + std::to_string(i) + " : Rotate").c_str());
		Vector3 scal = global->GetVector3Value(selectName, ("ItemNumber : " + std::to_string(i) + " : Scale").c_str());
		auto& item = items_.emplace_back(std::make_shared<RequiredItem>());
		item->transform.translate = trans;
		item->transform.rotate = rot;
		item->transform.scale = scal;
		item->Initialize();
	}
	
	num = global->GetIntValue(selectName, "CollectConfirmation");
	collects_.clear(); // 要素の全削除
	for (int i = 0; i < num; i++) {
		Vector3 trans = global->GetVector3Value(selectName, ("CollectNumber : " + std::to_string(i) + " : Translate").c_str());
		Quaternion rot = global->GetQuaternionValue(selectName, ("CollectNumber : " + std::to_string(i) + " : Rotate").c_str());
		Vector3 scal = global->GetVector3Value(selectName, ("CollectNumber : " + std::to_string(i) + " : Scale").c_str());
		auto& collect = collects_.emplace_back(std::make_shared<CollectionObject>());
		collect->transform.translate = trans;
		collect->transform.rotate = rot;
		collect->transform.scale = scal;
		collect->Initialize();
	}

	num = global->GetIntValue(selectName, "StageConfirmation");
	entrances_.clear(); // 要素の全削除
	for (int i = 0; i < num; i++) {
		Vector3 trans = global->GetVector3Value(selectName, ("StageNumber : " + std::to_string(i) + " : Translate").c_str());
		Quaternion rot = global->GetQuaternionValue(selectName, ("StageNumber : " + std::to_string(i) + " : Rotate").c_str());
		Vector3 scal = global->GetVector3Value(selectName, ("StageNumber : " + std::to_string(i) + " : Scale").c_str());
		auto& entrance = entrances_.emplace_back(std::make_shared<Entrance>());
		entrance->transform.translate = trans;
		entrance->transform.rotate = rot;
		entrance->transform.scale = scal;
		entrance->Initialize(i + 1);
	}
	
	goal_->transform.translate = global->GetVector3Value(selectName, "Goal : Translate");
	goal_->transform.rotate = global->GetQuaternionValue(selectName, "Goal : Rotate");
	goal_->transform.scale = global->GetVector3Value(selectName, "Goal : Scale");

	player_->transform.translate = global->GetVector3Value(selectName, "Player : Translate");
	player_->SetRespawnPos(player_->transform.translate);
	player_->transform.rotate = global->GetQuaternionValue(selectName, "Player : Rotate");
	player_->SetRespawnRot(player_->transform.rotate);
}

void Stage::StageSelectload(const std::filesystem::path& loadFile) {
	GlobalVariables* global = GlobalVariables::GetInstance();
	std::string selectName = loadFile.string();
	global->LoadFile(selectName);

	int num = global->GetIntValue(selectName, "BoxConfirmation");
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

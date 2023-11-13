#include "CreateStageScene.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"
#include "GlobalVariables/GlobalVariables.h"

void CreateStageScene::OnInitialize() {
	camera_ = std::make_shared<CameraAnimation>();
	camera_->Initialize();
	global_ = GlobalVariables::GetInstance();
	global_->CreateGroup(groupName);
}

void CreateStageScene::OnUpdate() {
	DrawImGui();
	global_->Update();

	for (auto& i : boxes_) {
		i->Update();
	}

	CollisionManager::GetInstance()->CheckCollision();
	// カメラの更新
	camera_->Update();
}

void CreateStageScene::OnFinalize() {

}

void CreateStageScene::DrawImGui() {
	ImGui::Begin("Stage", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Initialize")) {
			static Vector3 vec3 = Vector3::zero;
			ImGui::DragFloat3("transform", &vec3.x, 0.1f);
			if (ImGui::Button("Create")) {
				boxes_.push_back(std::make_shared<Box>());
				(*boxes_.rbegin())->Initialize(vec3, Vector3::one);
			}
			if (ImGui::Button("Save")) {
				global_->AddItem(groupName, "Confirmation : ", static_cast<int>(boxes_.size()));
				for (int i = 0; i < boxes_.size(); i++) {
					global_->AddItem(groupName, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str(), boxes_[i]->transform.scale);
					//global_->AddItem(groupName, ("BoxNumber : " + std::to_string(i + 1) + " : Rotate").c_str(), boxes_[i]->transform.rotate);
					global_->AddItem(groupName, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str(), boxes_[i]->transform.translate);
				}
				global_->SaveFile(groupName);
				global_->SaveMessage(groupName);
			}
			if (ImGui::Button("Load")) {
				global_->LoadFile(groupName);
				global_->LoadMessage(groupName);
				int num = global_->GetIntValue(groupName, "Confirmation : ");
				for (int i = 0; i < num; i++) {
					Vector3 trans = global_->GetVector3Value(groupName, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str());
					Vector3 scal = global_->GetVector3Value(groupName, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str());
					boxes_.push_back(std::make_shared<Box>());
					(*boxes_.rbegin())->Initialize(trans, scal);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Boxes")) {
			for (int i = 0; i < boxes_.size(); i++) {
				if (ImGui::TreeNode(("BoxNumber : " + std::to_string(i + 1)).c_str())) {
					boxes_[i]->DrawImGui();
					ImGui::TreePop();
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Confirmation")) {
			// 要素数確認
			ImGui::Text("ElementCount = %d", boxes_.size());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

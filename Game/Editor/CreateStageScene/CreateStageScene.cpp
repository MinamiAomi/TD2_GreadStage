#include "CreateStageScene.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"
#include "GlobalVariables/GlobalVariables.h"

void CreateStageScene::OnInitialize() {
	camera_ = std::make_shared<DebugCamera>();
	camera_->Initialize();
	global_ = GlobalVariables::GetInstance();
	global_->ChackFiles(fileName_);
	for (auto& i : fileName_) {
		global_->CreateGroup(i.c_str());
	}
	fileNumber_ = 0;
	loadSelectName_ = fileName_[fileNumber_].c_str();
}

void CreateStageScene::OnUpdate() {
	DrawImGui();

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
			if (ImGui::TreeNode("FileSave")) {
				ImGui::InputText("FileName", itemName_, sizeof(itemName_));
				if (ImGui::Button("Save")) {
					if (!boxes_.empty()) {
						global_->CreateGroup(itemName_);
						global_->AddItem(itemName_, "Confirmation : " + std::string(), static_cast<int>(boxes_.size()));
						for (int i = 0; i < boxes_.size(); i++) {
							global_->AddItem(itemName_, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str(), boxes_[i]->transform.scale);
							//global_->AddItem(groupName, ("BoxNumber : " + std::to_string(i + 1) + " : Rotate").c_str(), boxes_[i]->transform.rotate);
							global_->AddItem(itemName_, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str(), boxes_[i]->transform.translate);
						}
						global_->SaveFile(itemName_);
						global_->SaveMessage(itemName_);
						bool flag = false;
						for (auto& i : fileName_) {
							if (i.c_str() == std::string() + itemName_) {
								flag = true;
								break;
							}
						}
						if (!flag) {
							fileName_.push_back(itemName_);
						}
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("FileLoad")) {
				for (size_t i = 0; i < fileName_.size(); i++) {
					if (ImGui::RadioButton(fileName_[i].c_str(), &fileNumber_, static_cast<int>(i))) {
						loadSelectName_ = fileName_[fileNumber_].c_str();
					}
				}
				if (ImGui::Button("Load")) {
					global_->LoadFile(loadSelectName_);
					global_->LoadMessage(loadSelectName_);
					int num = global_->GetIntValue(loadSelectName_, "Confirmation : ");
					boxes_.clear(); // 要素の全削除
					for (int i = 0; i < num; i++) {
						Vector3 trans = global_->GetVector3Value(loadSelectName_, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str());
						Vector3 scal = global_->GetVector3Value(loadSelectName_, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str());
						boxes_.push_back(std::make_shared<Box>());
						(*boxes_.rbegin())->Initialize(trans, scal);
					}
				}
				ImGui::TreePop();
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

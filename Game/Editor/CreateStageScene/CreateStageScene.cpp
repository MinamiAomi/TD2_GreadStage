#include "CreateStageScene.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"

void CreateStageScene::OnInitialize() {
	camera_ = std::make_shared<CameraAnimation>();
	camera_->Initialize();
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
				(*boxes_.rbegin())->Initialize(vec3, 0.0f);
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

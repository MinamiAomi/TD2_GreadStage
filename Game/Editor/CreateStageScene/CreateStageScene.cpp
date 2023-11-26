#include "CreateStageScene.h"
#include "Graphics/ImGuiManager.h"
#include "Collision/CollisionManager.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Input/Input.h"

void CreateStageScene::OnInitialize() {
    debugCamera_ = std::make_shared<DebugCamera>();
    debugCamera_->Initialize();
    camera_ = std::make_shared<CameraAnimation>();
    camera_->Initialize();
    global_ = GlobalVariables::GetInstance();
    global_->ChackFiles(fileName_);
    for (auto& i : fileName_) {
        global_->CreateGroup(i.c_str());
    }
    fileNumber_ = 0;
    loadSelectName_ = fileName_[fileNumber_].c_str();

    stage_ = std::make_unique<Stage>();
    stage_->Initialize();

    player_ = std::make_shared<Player>();
    player_->Initialize();

    // セット
    stage_->SetPlayerPtr(player_);
    player_->SetCamera(camera_);
    camera_->SetTarget(&player_->transform);

}

void CreateStageScene::OnUpdate() {
    if (Input::GetInstance()->IsKeyTrigger(DIK_P)) {
        playFlg_ = !playFlg_;
    }
    DrawImGui();

    stage_->Update();

    if (playFlg_) {
        player_->Update();

        player_->PreCollisionUpdate();
        CollisionManager::GetInstance()->CheckCollision();
        player_->PostCollisionUpdate();

        // カメラの更新
        camera_->SetCamera();
        camera_->Update();
    }
    else {
        debugCamera_->SetCamera();
        debugCamera_->Update();
    }

    
    
}

void CreateStageScene::OnFinalize() {

}

void CreateStageScene::DrawImGui() {
	ImGui::Begin("Stage", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Initialize")) {
			if (ImGui::TreeNode("FileSave")) {
				ImGui::InputText("FileName", itemName_, sizeof(itemName_));
				if (ImGui::Button("Save")) {
					if (!stage_->GetBoxes().empty()) {
						global_->CreateGroup(itemName_);
						global_->SetValue(itemName_, "BoxConfirmation" + std::string(), static_cast<int>(stage_->GetBoxes().size()));
						for (int i = 0; i < stage_->GetBoxes().size(); i++) {
							global_->SetValue(itemName_, ("BoxNumber : " + std::to_string(i) + " : Scale").c_str(), stage_->GetBoxes()[i]->transform.scale);
							global_->SetValue(itemName_, ("BoxNumber : " + std::to_string(i) + " : Rotate").c_str(), stage_->GetBoxes()[i]->transform.rotate);
                            global_->SetValue(itemName_, ("BoxNumber : " + std::to_string(i) + " : Translate").c_str(), stage_->GetBoxes()[i]->transform.translate);
						}
                        global_->SetValue(itemName_, "ItemConfirmation" + std::string(), static_cast<int>(stage_->GetItems().size()));
						for (int i = 0; i < stage_->GetItems().size(); i++) {
							global_->SetValue(itemName_, ("ItemNumber : " + std::to_string(i) + " : Scale").c_str(), stage_->GetItems()[i]->transform.scale);
							global_->SetValue(itemName_, ("ItemNumber : " + std::to_string(i) + " : Rotate").c_str(), stage_->GetItems()[i]->transform.rotate);
                            global_->SetValue(itemName_, ("ItemNumber : " + std::to_string(i) + " : Translate").c_str(), stage_->GetItems()[i]->transform.translate);
						}
                        global_->SetValue(itemName_, "CollectConfirmation" + std::string(), static_cast<int>(stage_->GetCollects().size()));
						for (int i = 0; i < stage_->GetCollects().size(); i++) {
							global_->SetValue(itemName_, ("CollectNumber : " + std::to_string(i) + " : Scale").c_str(), stage_->GetCollects()[i]->transform.scale);
                            global_->SetValue(itemName_, ("CollectNumber : " + std::to_string(i) + " : Rotate").c_str(), stage_->GetCollects()[i]->transform.rotate);
                            global_->SetValue(itemName_, ("CollectNumber : " + std::to_string(i) + " : Translate").c_str(), stage_->GetCollects()[i]->transform.translate);
						}
                        global_->SetValue(itemName_, "StageConfirmation" + std::string(), static_cast<int>(stage_->GetEntrance().size()));
						for (int i = 0; i < stage_->GetEntrance().size(); i++) {
							global_->SetValue(itemName_, ("StageNumber : " + std::to_string(i) + " : Scale").c_str(), stage_->GetEntrance()[i]->transform.scale);
                            global_->SetValue(itemName_, ("StageNumber : " + std::to_string(i) + " : Rotate").c_str(), stage_->GetEntrance()[i]->transform.rotate);
                            global_->SetValue(itemName_, ("StageNumber : " + std::to_string(i) + " : Translate").c_str(), stage_->GetEntrance()[i]->transform.translate);
						}
                        global_->SetValue(itemName_, "Goal : Translate" + std::string(), stage_->GetGoal()->transform.translate);
                        global_->SetValue(itemName_, "Goal : Rotate" + std::string(), stage_->GetGoal()->transform.rotate);
						global_->SetValue(itemName_, "Player : Translate" + std::string(), stage_->GetPlayer()->transform.translate);
                        global_->SetValue(itemName_, "Player : Rotate" + std::string(), stage_->GetPlayer()->transform.rotate);
						
                        global_->SaveFile(itemName_);
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
                    stage_->Load(loadSelectName_);
                }
                ImGui::TreePop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Boxes")) {
            if (ImGui::Button("Create")) {
                auto box = std::make_shared<Box>();
                stage_->Add(box);
            }
            // 要素数確認
            ImGui::Text("ElementCount = %d", stage_->GetBoxes().size());
            for (int i = 0; i < stage_->GetBoxes().size(); i++) {
                if (ImGui::TreeNode(("BoxNumber : " + std::to_string(i)).c_str())) {
                    stage_->GetBoxes()[i]->DrawImGui();
                    if (ImGui::Button("Delete")) {
                        stage_->DeleteBox(i);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("RequiredItem")) {
            if (ImGui::Button("Create")) {
                auto item = std::make_shared<RequiredItem>();
                stage_->Add(item);
            }
            // 要素数確認
            ImGui::Text("ElementCount = %d", stage_->GetItems().size());
            for (int i = 0; i < stage_->GetItems().size(); i++) {
                if (ImGui::TreeNode(("ItemNumber : " + std::to_string(i)).c_str())) {
                    stage_->GetItems()[i]->DrawImGui();
                    if (ImGui::Button("Delete")) {
                        stage_->DeleteItem(i);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("CollectionObject")) {
            if (ImGui::Button("Create")) {
                auto collect = std::make_shared<CollectionObject>();
                stage_->Add(collect);
            }
            // 要素数確認
            ImGui::Text("ElementCount = %d", stage_->GetCollects().size());
            for (int i = 0; i < stage_->GetCollects().size(); i++) {
                if (ImGui::TreeNode(("ItemNumber : " + std::to_string(i)).c_str())) {
                    stage_->GetCollects()[i]->DrawImGui();
                    if (ImGui::Button("Delete")) {
                        stage_->DeleteCollect(i);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("OnlyOneObject")) {
            if (ImGui::TreeNode("Player")) {
                ImGui::DragFloat3("trans", &stage_->GetPlayer()->transform.translate.x, 0.1f);
                static Vector3 pRot = Vector3::zero;
                ImGui::DragFloat3("rotate", &pRot.x, 0.1f, -360.0f, 360.0f);
                stage_->GetPlayer()->transform.rotate = Quaternion::MakeFromEulerAngle(pRot * Math::ToRadian);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Goal")) {
                stage_->GetGoal()->DrawImGui();
                ImGui::TreePop();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("StageSelect")) {
            if (ImGui::Button("Create")) {
                auto item = std::make_shared<Entrance>();
                stage_->Add(item);
            }
            // 要素数確認
            ImGui::Text("ElementCount = %d", stage_->GetEntrance().size());
            for (int i = 0; i < stage_->GetEntrance().size(); i++) {
                if (ImGui::TreeNode(("StageNumber : " + std::to_string(i)).c_str())) {
                    stage_->GetEntrance()[i]->DrawImGui();
                    if (ImGui::Button("Delete")) {
                        stage_->DeleteEntrance(i);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

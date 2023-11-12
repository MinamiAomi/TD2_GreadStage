#include "Editor.h"
#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Engine/Graphics/ToonModel.h"

#include "Game/Editor/CreateStageScene/CreateStageScene.h"

void Editor::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<CreateStageScene>();

	// リソースマネージャーのシングルトンの取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	// モデルの追加
	std::shared_ptr<ToonModel> toonModel = std::make_shared<ToonModel>();
	// 箱
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/box.obj"));
	resourceManager->AddToonModel("Box", toonModel);

}

void Editor::OnFinalize() {

}

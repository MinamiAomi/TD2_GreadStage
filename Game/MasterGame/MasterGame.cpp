#include "MasterGame.h"

#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Sprite.h"

#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/BattleScene/BattleScene.h"

void MasterGame::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<BattleScene>();

	// リソースマネージャーのシングルトンの取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	// モデルの追加
	std::shared_ptr<Model> toonModel = std::make_shared<Model>();
	// プレイヤー
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/player.obj"));
	resourceManager->AddToonModel("Player", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/box.obj"));
	resourceManager->AddToonModel("Box", toonModel);

}

void MasterGame::OnFinalize() {

}

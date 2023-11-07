#include "MasterGame.h"

#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Engine/Graphics/ToonModel.h"

#include "BattleScene/BattleScene.h"

void MasterGame::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<BattleScene>();

	// リソースマネージャーのシングルトンの取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	// モデルの追加
	std::shared_ptr<ToonModel> toonModel = std::make_shared<ToonModel>();;
	// プレイヤー
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/player.obj"));
	resourceManager->AddToonModel("Player", toonModel);
	// 地面
	toonModel = std::make_shared<ToonModel>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/floor.obj"));
	resourceManager->AddToonModel("Floor", toonModel);
	// 壁
	toonModel = std::make_shared<ToonModel>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Wall/wall.obj"));
	resourceManager->AddToonModel("Wall", toonModel);

}

void MasterGame::OnFinalize() {

}
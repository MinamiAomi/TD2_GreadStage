#include "MasterGame.h"

#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Sprite.h"

#include "Game/Scene/TitleScene/TitleScene.h"

void MasterGame::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<TitleScene>();

	// リソースマネージャーのシングルトンの取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	// モデルの追加
	std::shared_ptr<Model> toonModel = std::make_shared<Model>();
	// プレイヤー
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/player.obj"));
	resourceManager->AddToonModel("Player", toonModel);
	// 地面
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/floor.obj"));
	resourceManager->AddToonModel("Floor", toonModel);
	// 壁
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Wall/wall.obj"));
	resourceManager->AddToonModel("Wall", toonModel);

	std::shared_ptr<Texture> sprite = std::make_shared<Texture>();
	sprite->Load("Resources/Texture/title.png");
	

}

void MasterGame::OnFinalize() {

}

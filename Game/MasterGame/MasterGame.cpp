#include "MasterGame.h"

#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Sprite.h"

#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/BattleScene/BattleScene.h"
#include "Editor/CreateStageScene/CreateStageScene.h"

void MasterGame::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<TitleScene>();
	//sceneManager->ChangeScene<CreateStageScene>();

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

	// プレイヤー
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/Body.obj"));
	resourceManager->AddToonModel("YamoriBody", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/Head.obj"));
	resourceManager->AddToonModel("YamoriHead", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/LeftHand.obj"));
	resourceManager->AddToonModel("YamoriLeftHand", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/RightHand.obj"));
	resourceManager->AddToonModel("YamoriRightHand", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/LeftFoot.obj"));
	resourceManager->AddToonModel("PlayerLeftFoot", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Yamori/RightFoot.obj"));
	resourceManager->AddToonModel("PlayerRightFoot", toonModel);
	
	// 収集物
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Crystal/crystal.obj"));
	resourceManager->AddToonModel("Crystal", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Star/star.obj"));
	resourceManager->AddToonModel("Star", toonModel);

}

void MasterGame::OnFinalize() {

}

#include "MasterGame.h"

#include "Scene/SceneManager.h"
#include "Engine/Graphics/ResourceManager.h"
#include "Graphics/RenderManager.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Sprite.h"
#include "Graphics/RenderManager.h"

#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game/Scene/BattleScene/BattleScene.h"
#include "Editor/CreateStageScene/CreateStageScene.h"
#include "Transition/Transition.h"

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

	// タイトル用板ポリゴン
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/TitleText/titleText.obj"));
	resourceManager->AddToonModel("TitleText", toonModel);

	// プレイヤー
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/Body.obj"));
	resourceManager->AddToonModel("FrogBody", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/Head.obj"));
	resourceManager->AddToonModel("FrogHead", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/leftarm.obj"));
	resourceManager->AddToonModel("FrogLeftArm", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/rightarm.obj"));
	resourceManager->AddToonModel("FrogRightArm", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/leftfoot.obj"));
	resourceManager->AddToonModel("FrogLeftFoot", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/rightfoot.obj"));
	resourceManager->AddToonModel("FrogRightFoot", toonModel);
	
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/frog/bag.obj"));
	resourceManager->AddToonModel("FrogBag", toonModel);

	// 収集物
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Crystal/crystal.obj"));
	resourceManager->AddToonModel("Crystal", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/Star/star.obj"));
	resourceManager->AddToonModel("Star", toonModel);

	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/moon/moon.obj"));
	resourceManager->AddToonModel("Moon", toonModel);
  
	toonModel = std::make_shared<Model>();
	toonModel->Create(ModelData::LoadObjFile("Resources/Model/GoalWell/GoalWell.obj"));
	resourceManager->AddToonModel("GoalWell", toonModel);
  
  
	std::shared_ptr<Texture> sprite = std::make_shared<Texture>();
	sprite->Load("Resources/Texture/block.png");
	resourceManager->AddTexture("Block", sprite);

	// トランジション用初期化
	auto trans = Transition::GetInstance();
	trans->Initialize();
	// ガウシアンブラーのオフ
	RenderManager::GetInstance()->UseGaussianBlur(false);


}

void MasterGame::OnFinalize() {

}

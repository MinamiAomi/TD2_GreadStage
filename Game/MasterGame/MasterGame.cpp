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
#include "Audio/Audio.h"
#include "Transition/Transition.h"

void MasterGame::OnInitialize() {
	// シーンのシングルトンの取得
	SceneManager* sceneManager = SceneManager::GetInstance();
	// シーンの設定
	sceneManager->ChangeScene<TitleScene>();
	//sceneManager->ChangeScene<CreateStageScene>();

	// リソースマネージャーのシングルトンの取得
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	Audio* audio = Audio::GetInstance();
	// モデルの追加
	std::shared_ptr<Model> model = std::make_shared<Model>();
	// プレイヤー
	model->Create(ModelData::LoadObjFile("Resources/Model/player.obj"));
	resourceManager->AddToonModel("Player", model);

	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/box.obj"));
	resourceManager->AddToonModel("Box", model);

	// タイトル用板ポリゴン
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/title/title.obj"));
	resourceManager->AddToonModel("TitleText", model);

	// プレイヤー
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/Body.obj"));
	resourceManager->AddToonModel("FrogBody", model);

	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/Head.obj"));
	resourceManager->AddToonModel("FrogHead", model);

	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/leftarm.obj"));
	resourceManager->AddToonModel("FrogLeftArm", model);
	
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/rightarm.obj"));
	resourceManager->AddToonModel("FrogRightArm", model);
	
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/leftfoot.obj"));
	resourceManager->AddToonModel("FrogLeftFoot", model);
	
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/rightfoot.obj"));
	resourceManager->AddToonModel("FrogRightFoot", model);
	
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/frog/bag.obj"));
	resourceManager->AddToonModel("FrogBag", model);

	// 収集物
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/Crystal/crystal.obj"));
	resourceManager->AddToonModel("Crystal", model);

	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/Star/star.obj"));
	resourceManager->AddToonModel("Star", model);

	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/moon/moon.obj"));
	resourceManager->AddToonModel("Moon", model);
  
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/GoalWell/GoalWell.obj"));
	resourceManager->AddToonModel("GoalWell", model);
  
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/StartWell/startWell.obj"));
	resourceManager->AddToonModel("StartWell", model);
  
	model = std::make_shared<Model>();
	model->Create(ModelData::LoadObjFile("Resources/Model/StartWell/board.obj"));
	resourceManager->AddToonModel("StartWellBoard", model);
  
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/block.png");
	resourceManager->AddTexture("Block", texture);

	// ポーズ用
	texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/Pause/stageSelect.png");
	resourceManager->AddTexture("StageSelect", texture);
	
	texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/Pause/restart.png");
	resourceManager->AddTexture("Restart", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/Pause/pose.png");
	resourceManager->AddTexture("Pose", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/Pause/controller.png");
	resourceManager->AddTexture("Controller", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Texture/Pause/pose_backGround.png");
	resourceManager->AddTexture("BackGround", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board1.png");
	resourceManager->AddTexture("Board1", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board2.png");
	resourceManager->AddTexture("Board2", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board3.png");
	resourceManager->AddTexture("Board3", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board4.png");
	resourceManager->AddTexture("Board4", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board5.png");
	resourceManager->AddTexture("Board5", texture);

	texture = std::make_shared<Texture>();
	texture->Load("Resources/Model/StartWell/board6.png");
	resourceManager->AddTexture("Board6", texture);

	resourceManager->AddSound("BGM", audio->SoundLoadWave("Resources/Sound/bgm.wav"));
	resourceManager->AddSound("GravityChange", audio->SoundLoadWave("Resources/Sound/gravityChange.wav"));
	resourceManager->AddSound("Jump", audio->SoundLoadWave("Resources/Sound/jump.wav"));
	resourceManager->AddSound("MoonGet", audio->SoundLoadWave("Resources/Sound/moonGet.wav"));
	resourceManager->AddSound("StarGet", audio->SoundLoadWave("Resources/Sound/starGet.wav"));
	resourceManager->AddSound("Walk", audio->SoundLoadWave("Resources/Sound/stepSound.wav"));
	resourceManager->AddSound("Landing", audio->SoundLoadWave("Resources/Sound/landing.wav"));

	// トランジション用初期化
	auto trans = Transition::GetInstance();
	trans->Initialize();
	// ガウシアンブラーのオフ
	RenderManager::GetInstance()->UseGaussianBlur(false);
}

void MasterGame::OnFinalize() {

}

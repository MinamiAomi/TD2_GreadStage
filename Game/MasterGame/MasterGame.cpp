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

    auto LoadModel = [resourceManager](const std::string& name, const std::filesystem::path& path) {
        std::shared_ptr<Model> model = std::make_shared<Model>();
        model->Create(ModelData::LoadObjFile(path));
        resourceManager->AddToonModel(name, model);
        };

    auto LoadTexture = [resourceManager](const std::string& name, const std::filesystem::path& path) {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>();
        texture->Load(path);
        resourceManager->AddTexture(name, texture);
        };

    auto LoadSound = [resourceManager, audio](const std::string& name, const std::filesystem::path& path) {
        resourceManager->AddSound(name, audio->SoundLoadWave(path.string().c_str()));
        };

    // 箱
    LoadModel("Box", "Resources/Model/box.obj");
    // タイトル
    LoadModel("TitleText", "Resources/Model/title/title.obj");
    // カエルからだ
    LoadModel("FrogBody", "Resources/Model/frog/Body.obj");
    // カエル頭
    LoadModel("FrogHead", "Resources/Model/frog/Head.obj");
    // カエル左腕
    LoadModel("FrogLeftArm", "Resources/Model/frog/leftarm.obj");
    // カエル右腕
    LoadModel("FrogRightArm", "Resources/Model/frog/rightarm.obj");
    // カエル左脚
    LoadModel("FrogLeftFoot", "Resources/Model/frog/leftfoot.obj");
    // カエル右脚
    LoadModel("FrogRightFoot", "Resources/Model/frog/rightfoot.obj");
    // カエルカバン
    LoadModel("FrogBag", "Resources/Model/frog/bag.obj");
    // 収集物星
    LoadModel("Star", "Resources/Model/Star/star.obj");
    // 収集物月
    LoadModel("Moon", "Resources/Model/moon/moon.obj");
    // ゴール井戸
    LoadModel("GoalWell", "Resources/Model/GoalWell/GoalWell.obj");
    // スタート井戸
    LoadModel("StartWell", "Resources/Model/StartWell/startWell.obj");
    // スタート井戸看板
    LoadModel("StartWellBoard", "Resources/Model/StartWell/board.obj");

    // ブロック
    LoadTexture("Block", "Resources/Texture/block.png");
    // ステージセレクト画像
    LoadTexture("StageSelect", "Resources/Texture/Pause/stageSelect.png");
    // リスタート画像
    LoadTexture("Restart", "Resources/Texture/Pause/restart.png");
    // ポーズ画像
    LoadTexture("Pose", "Resources/Texture/Pause/pose.png");
    // コントローラー
    LoadTexture("Controller", "Resources/Texture/Pause/controller.png");
    // ポーズ時の背景
    LoadTexture("BackGround", "Resources/Texture/Pause/pose_backGround.png");
    // セレクト井戸のボード1
    LoadTexture("Board1", "Resources/Model/StartWell/board1.png");
    // セレクト井戸のボード2
    LoadTexture("Board2", "Resources/Model/StartWell/board2.png");
    // セレクト井戸のボード3
    LoadTexture("Board3", "Resources/Model/StartWell/board3.png");
    // セレクト井戸のボード4
    LoadTexture("Board4", "Resources/Model/StartWell/board4.png");
    // セレクト井戸のボード5
    LoadTexture("Board5", "Resources/Model/StartWell/board5.png");
    // セレクト井戸のボード6
    LoadTexture("Board6", "Resources/Model/StartWell/board6.png");
    //フレーム
    LoadTexture("Frame", "Resources/Texture/Pause/frame.png");
    // 収集物 月
    LoadTexture("Moon", "Resources/Texture/Pause/moon.png");
    // オプション
    LoadTexture("Option", "Resources/Texture/Pause/option.png");


    LoadTexture("Ground00", "Resources/Texture/Ground/ground00.png");

    // BGM
    LoadSound("BGM", "Resources/Sound/bgm.wav");
    // ジャンプ
    LoadSound("Jump", "Resources/Sound/jump.wav");
    // 月取得時
    LoadSound("MoonGet", "Resources/Sound/moonGet.wav");
    // 星取得時
    LoadSound("StarGet", "Resources/Sound/starGet.wav");
    // 歩く音
    LoadSound("Walk", "Resources/Sound/stepSound.wav");
    // 着地音
    LoadSound("Landing", "Resources/Sound/landing.wav");

    // トランジション用初期化
    auto trans = Transition::GetInstance();
    trans->Initialize();
    // ガウシアンブラーのオフ
    RenderManager::GetInstance()->UseGaussianBlur(false);
}

void MasterGame::OnFinalize() {

}

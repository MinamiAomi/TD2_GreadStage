#include "BattleScene.h"

void BattleScene::OnInitialize() {
	// 生成
	player_ = std::make_shared<Player>();

	// 初期化
	player_->Initialize();

}

void BattleScene::OnUpdate() {
	player_->Update();
}

void BattleScene::OnFinalize() {

}

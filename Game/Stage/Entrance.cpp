#include "Entrance.h"

void Entrance::Initialize() {

}

void Entrance::Update() {

}

void Entrance::OnCollision(const CollisionInfo& collisionInfo) {
    if (collisionInfo.collider->GetName() == "Player") {
        
    }
}

void Entrance::DrawImGui() {

}

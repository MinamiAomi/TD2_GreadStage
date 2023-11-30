#include "Goal.h"
#include "Graphics/ResourceManager.h"
#include "Graphics/ImGuiManager.h"
#include "CollisionConfig.h"
#include "Graphics/ParticleManager.h"

void Goal::Initialize() {
    SetName("Goal");
    model_ = std::make_unique<ModelInstance>();
    collider_ = std::make_unique<BoxCollider>();

    transform.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform.UpdateMatrix();

    model_->SetModel(ResourceManager::GetInstance()->FindModel("GoalWell"));
    model_->SetIsActive(true);

    collider_->SetName("Goal");
    collider_->SetCollisionAttribute(CollisionConfig::Goal);
    collider_->SetCollisionMask(CollisionConfig::Player);
    collider_->SetGameObject(this);
    collider_->SetOrientation(transform.rotate);
    collider_->SetSize(transform.scale);
    collider_->SetCenter(transform.translate);
    model_->SetColor(Vector3::one);
}

void Goal::Update(uint32_t currentItemCount) {

    if (currentItemCount == 0) {
        const uint32_t numParticles = 3;
        std::list<Particle> particles(numParticles);

        Vector3 base = transform.worldMatrix.GetTranslate();

        for (auto& particle : particles) {
            particle.position = base;

            particle.velocity.x = ranGen_.NextFloatRange(-1.0f, 1.0f) * 0.2f;
            particle.velocity.y = ranGen_.NextFloatRange(0.0f, 1.0f) * 2.0f;
            particle.velocity.z = ranGen_.NextFloatRange(-1.0f, 1.0f) * 0.2f;
            particle.velocity = particle.velocity * 0.2f;
            particle.velocity = transform.rotate * particle.velocity;

            particle.startSize = 0.2f;
            particle.endSize = 0.1f;
            particle.startColor = Vector3::unitZ;
            particle.endColor = Vector3::unitZ;
            particle.startAlpha = 1.0f;
            particle.endAlpha = 0.0f;
            particle.existenceTime = 0;
            particle.lifeTime = 60;
        }

        ParticleManager::GetInstance()->AddParticles(std::move(particles));
    }


    transform.scale = Vector3::one;
    // 当たり判定、描画を更新
    transform.UpdateMatrix();
    collider_->SetOrientation(transform.rotate);
    collider_->SetCenter(Vector3{ 0.0f, 0.5f, 0.0f } *transform.worldMatrix);
    model_->SetWorldMatrix(transform.worldMatrix);
}

void Goal::DrawImGui() {
#ifdef _DEBUG
    ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &rotate_.x, 0.1f, -360.0f, 360.0f);
    transform.rotate = Quaternion::MakeFromEulerAngle(rotate_ * Math::ToRadian);
    ImGui::DragFloat3("translate", &transform.translate.x, 0.1f);
#endif // DEBUG
}

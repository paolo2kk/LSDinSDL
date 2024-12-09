#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "PerfTimer.h" 
#include "EntityManager.h"

Enemy::Enemy() : Entity(EntityType::BOX)
{
    name = "Enemy";
    perfTimer = PerfTimer();
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {
    return true;
}

bool Enemy::Start() {
    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
    texW = parameters.attribute("w").as_int();
    texH = parameters.attribute("h").as_int();

    right.LoadAnimations(parameters.child("animations").child("right"));
    left.LoadAnimations(parameters.child("animations").child("left"));
    currentAnimation = &right;

    Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
    pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW, texH, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
    pbody->body->SetFixedRotation(true);
    pbody->body->SetGravityScale(0.0f);
    pbody->body->SetBullet(true);
    velocity.Set(0,0);
    perfTimer.Start();

    return true;
}

void Enemy::GravityChange(b2Vec2& velocity)
{
    auto input = Engine::GetInstance().input.get();
    /*if (canExecuteGrav)
    {*/
    if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT ||
        input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT) {
        velocity = b2Vec2(0, GRAVITY_Y);
    }
    else if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT ||
        input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT) {
        velocity = b2Vec2(0, -GRAVITY_Y);
    }
    else if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT ||
        input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT) {
        velocity = b2Vec2(GRAVITY_Y, 0);
        currentAnimation = &right;
    }
    else if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT ||
        input->GetControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT) {
        velocity = b2Vec2(-GRAVITY_Y, 0);
        currentAnimation = &left;
    }


     /*   canExecuteGrav = false;
    }*/

}

bool Enemy::Update(float dt)
{
    /*double elapsedMs = perfTimer.ReadMs();

    gravTimer += elapsedMs;
    inputBufferTimer -= elapsedMs;

    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_UP) ||
        Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_DOWN) ||
        Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LEFT) ||
        Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RIGHT))
    {
        inputBufferTimer = inputBufferTime;
    }

    if (gravTimer >= gravDesiredTimer && inputBufferTimer > 0)
    {
        GravityChange(velocity);
        gravTimer = 0;
    }*/

    if (canMove)
    {
        GravityChange(velocity);

    }


      /*  if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
            velocity.x = 0.2 * dt;
        }
        if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
            velocity.x = -0.2 * dt;
        }
        if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
            velocity.y = -0.2 * dt;
        }
        if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
            velocity.y = 0.2 * dt;
        }
   */

    
    
    pbody->body->SetLinearVelocity(velocity);

    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());
    return true;

    if (toDestroy == true)
    {
        Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
        Engine::GetInstance().entityManager->DestroyEntity(this);
    }
}

bool Enemy::CleanUp()
{
    LOG("Cleanup player");
    Engine::GetInstance().textures.get()->UnLoad(texture);
    return true;
}

// Collision handling
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype)
    {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        canExecuteGrav = true;

        break;
    case ColliderType::ITEM:
        LOG("Collision ITEM");
        break;
    case ColliderType::UNKNOWN:
        LOG("Collision UNKNOWN");
        break;
    case ColliderType::TRIGGER:
        LOG("Trigger Collided");
        inTrigger = true;
        break;
    default:
        break;
    }
}

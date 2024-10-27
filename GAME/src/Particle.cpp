#include "Particle.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "PerfTimer.h" 

Particle::Particle() : Entity(EntityType::PARTICLE)
{
    name = "Particle";
    perfTimer = PerfTimer();
}

Particle::~Particle() {

}

bool Particle::Awake() {
    position = Vector2D(0, 0);
    return true;
}

bool Particle::Start() {
    texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Particle1.png");
    Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
    pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW, texH, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::PLAYER;
    pbody->body->SetFixedRotation(true);
    pbody->body->SetGravityScale(0.0f);

    perfTimer.Start();

    return true;
}

void Particle::GravityChange(b2Vec2& velocity)
{
    auto input = Engine::GetInstance().input.get();
    /*if (canExecuteGrav)
    {*/
        if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
            velocity = b2Vec2(0, GRAVITY_Y);
        }
        else if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
            velocity = b2Vec2(0, -GRAVITY_Y);
        }
        else if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
            velocity = b2Vec2(GRAVITY_Y, 0);
        }
        else if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
            velocity = b2Vec2(-GRAVITY_Y, 0);
        }
     /*   canExecuteGrav = false;
    }*/

}

bool Particle::Update(float dt)
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
}

bool Particle::CleanUp()
{
    LOG("Cleanup player");
    Engine::GetInstance().textures.get()->UnLoad(texture);
    return true;
}

// Collision handling
void Particle::OnCollision(PhysBody* physA, PhysBody* physB) {
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

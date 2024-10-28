#include "MovingPlatform.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "PerfTimer.h" 

MovingPlatform::MovingPlatform(bool movingLeftt, bool movingRightt) : Entity(EntityType::BOX)
{
    name = "MovingPlatform";
    perfTimer = PerfTimer();
    movingLeft = movingLeftt;
    movingRight = movingRightt;
}

MovingPlatform::~MovingPlatform() {

}

bool MovingPlatform::Awake() {
   return true;
}

bool MovingPlatform::Start() {
    texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/MovingPlatform.png");
    Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
    pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), texW, texH, bodyType::KINEMATIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::BOX;
    pbody->body->SetFixedRotation(true);
    pbody->body->SetGravityScale(0.0f);

    perfTimer.Start();
    positionBloq = position.getX();

    return true;
}

void MovingPlatform::moveFromLeftToRight()
{

}

bool MovingPlatform::Update(float dt)
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
        if (movingLeft)
        {
            velocity.x = -0.4 * dt; 
            velocity.y = 0; 
            if (position.getX() <= 1000) {
                movingLeft = false;
                movingRight = true;
            }
        }
        else if (movingRight)
        {
            velocity.x = 0.4f * dt;
            velocity.y = 0;
            if (position.getX() >= 1600) {
                movingLeft = true;
                movingRight = false;
            }
        }
    }
    else
    {
        velocity.x = 0; 
    }
      /* if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
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
    
    pbody->body->SetLinearVelocity(b2Vec2(velocity.x, 0.0f));
    
    b2Vec2 fixedPosition = (pbody->body->GetPosition());
    
    //pbody->body->SetTransform(fixedPosition, pbody->body->GetAngle());

    position.setX(METERS_TO_PIXELS(fixedPosition.x) - texW / 2);
    position.setY(METERS_TO_PIXELS(fixedPosition.y) - texH / 2);
    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY());
    return true;
}

bool MovingPlatform::CleanUp()
{
    LOG("Cleanup player");
    Engine::GetInstance().textures.get()->UnLoad(texture);
    return true;
}

// Collision handling
void MovingPlatform::OnCollision(PhysBody* physA, PhysBody* physB) {
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

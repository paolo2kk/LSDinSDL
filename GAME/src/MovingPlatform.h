// MovingPlatform.h

#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Physics.h"
#include "PerfTimer.h"  // Include the PerfTimer header

struct SDL_Texture;

class MovingPlatform : public Entity
{
public:
    MovingPlatform(bool movingLeftt, bool movingRightt);
    virtual ~MovingPlatform();

    bool Awake();
    bool Start();
    void moveFromLeftToRight();
    bool Update(float dt);
    bool CleanUp();
    
    void OnCollision(PhysBody* physA, PhysBody* physB);

public:
    // MovingPlatform parameters
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW, texH;
    bool inTrigger = false;
    // Audio fx
    int pickCoinFxId;

    // Physics body
    PhysBody* pbody;
    void SetPosition(Vector2D newPosition) {
        position = newPosition;
        pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(newPosition.getX()), PIXEL_TO_METERS(newPosition.getY())), 0);
    }
    // Gravity modifiers
    b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
    float gravTimer = 0;
    float gravDesiredTimer = 3000;
    float inputBufferTime = 200;
    float inputBufferTimer = 0;
    bool canExecuteGrav = true;
    bool movingLeft = false;
    bool movingRight = false;

    int positionBloq;

    PerfTimer perfTimer;
};

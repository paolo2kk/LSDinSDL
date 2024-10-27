// Box.h

#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Physics.h"
#include "PerfTimer.h"  // Include the PerfTimer header

struct SDL_Texture;

class Box : public Entity
{
public:
    Box();
    virtual ~Box();

    bool Awake();
    bool Start();
    void GravityChange(b2Vec2& velocity);
    bool Update(float dt);
    bool CleanUp();
    
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void SetPosition(Vector2D newPosition) {
        position = newPosition;
        pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(newPosition.getX()), PIXEL_TO_METERS(newPosition.getY())), 0);
    }
public:
    // Box parameters
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW, texH;
    bool inTrigger = false;
    // Audio fx
    int pickCoinFxId;

    // Physics body
    PhysBody* pbody;

    // Gravity modifiers
    b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
    float gravTimer = 0;
    float gravDesiredTimer = 3000;
    float inputBufferTime = 200;
    float inputBufferTimer = 0;
    bool canExecuteGrav = true;

    PerfTimer perfTimer;
};

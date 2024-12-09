#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "Module.h"
#include "Player.h"

class ParallaxLayer {
public:
    ParallaxLayer(SDL_Texture* texture, int speed);
    virtual ~ParallaxLayer() = default;
    void render(SDL_Renderer* renderer, int cameraX);

private:
    SDL_Texture* texture; 
    int speed;            
    int width;            
};
class Player;
class ParallaxBackground : public Module {
public:
    ParallaxBackground();
    ~ParallaxBackground();

    bool Start();
    //bool PreUpdate();
    bool Update(float dt);
    //bool PostUpdate();
    //bool CleanUp();
    void addLayer(SDL_Texture* texture, int speed);
    void updateAndRender();
    void setRenderer(SDL_Renderer* renderer) { this->renderer = renderer; }
    void setCameraX(int cameraX) {
         this->cameraX = cameraX;
    }
    void setPlayer(Player* player) { this->player = player; }

    void setPlayerPosition(int playerX) { this->cameraX = playerX; }
private:
    SDL_Renderer* renderer = nullptr;  
    int cameraX = 0;
    std::vector<ParallaxLayer> layers; 
    SDL_Texture* bgTexture1;
    SDL_Texture* bgTexture2;
    Player* player = nullptr; 
};


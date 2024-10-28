#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "Module.h"

class ParallaxLayer {
public:
    ParallaxLayer(SDL_Texture* texture, int speed);
    virtual ~ParallaxLayer() = default;
    void render(SDL_Renderer* renderer, int cameraX);

private:
    SDL_Texture* texture; // Textura de la capa
    int speed;            // Velocidad de desplazamiento
    int width;            // Ancho de la textura
};

class ParallaxBackground : public Module {
public:
    ParallaxBackground();
    ~ParallaxBackground();

    bool Start();
    //bool PreUpdate();
    bool Update(float dt);
    //bool PostUpdate();
    //bool CleanUp();
    void addLayer(SDL_Texture* texture, int speed); // Agrega una capa
    void updateAndRender();
    void setRenderer(SDL_Renderer* renderer) { this->renderer = renderer; }
    void setCameraX(int cameraX) {
         this->cameraX = cameraX;
    }
private:
    SDL_Renderer* renderer = nullptr;  // Store renderer as a member variable
    int cameraX = 0;  // Store camera X position as a member variable
    std::vector<ParallaxLayer> layers; // Vector de capas de parallax

    SDL_Texture* bgTexture1;
    SDL_Texture* bgTexture2;
};


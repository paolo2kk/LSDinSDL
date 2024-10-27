#pragma once

#include <SDL2/SDL.h>
#include <vector>

class ParallaxLayer {
public:
    ParallaxLayer(SDL_Texture* texture, int speed);
    void render(SDL_Renderer* renderer, int cameraX);

private:
    SDL_Texture* texture; // Textura de la capa
    int speed;            // Velocidad de desplazamiento
    int width;            // Ancho de la textura
};

class ParallaxBackground {
public:
    ParallaxBackground();
    ~ParallaxBackground();

    void addLayer(SDL_Texture* texture, int speed); // Agrega una capa
    void updateAndRender(SDL_Renderer* renderer, int cameraX); // Actualiza y renderiza todas las capas

private:
    std::vector<ParallaxLayer> layers; // Vector de capas de parallax
};


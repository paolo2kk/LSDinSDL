#include "ParallaxBackground.h"
#include <SDL2/SDL.h>


// Constructor para la clase ParallaxLayer
ParallaxLayer::ParallaxLayer(SDL_Texture* texture, int speed)
    : texture(texture), speed(speed) {
    // Obtenemos el ancho de la textura
    SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
}

// Renderiza una capa de parallax
void ParallaxLayer::render(SDL_Renderer* renderer, int cameraX) {
    int renderX = -(cameraX / speed) % width; // Calcula la posición de la textura

    SDL_Rect srcRect = { 0, 0, width, 480 }; // Ajustar según el tamaño de la capa
    SDL_Rect destRect = { renderX, 0, width, 480 };

    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

    // Si el fondo no llena la pantalla, renderizamos una segunda vez
    if (renderX + width < 800) {
        destRect.x = renderX + width;
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }
}

// Constructor de ParallaxBackground
ParallaxBackground::ParallaxBackground() {}

// Destructor de ParallaxBackground
ParallaxBackground::~ParallaxBackground() {
    // No hacemos nada ya que SDL_DestroyTexture debería ser manejado externamente
}

// Agrega una nueva capa al vector de capas
void ParallaxBackground::addLayer(SDL_Texture* texture, int speed) {
    layers.emplace_back(texture, speed);
}

// Actualiza y renderiza todas las capas del fondo parallax
void ParallaxBackground::updateAndRender(SDL_Renderer* renderer, int cameraX) {
    for (ParallaxLayer& layer : layers) {
        layer.render(renderer, cameraX);
    }
}
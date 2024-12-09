#include "ParallaxBackground.h"
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include <SDL2/SDL.h>
#include "Log.h"

bool ParallaxBackground::Start() {

    bgTexture1 = Engine::GetInstance().textures.get()->Load("Assets/Textures/layer1.png");
    bgTexture2 = Engine::GetInstance().textures.get()->Load("Assets/Textures/layer2.png");
    //parallax.addLayer(bgTexture1, 5);  // Capa 1 con velocidad 5
    //parallax.addLayer(bgTexture2, 10); // Capa 2 con velocidad 10
    //parallax.updateAndRender(Engine::GetInstance().render->renderer, cameraX);
    return true;
}

// Constructor para la clase ParallaxLayer
ParallaxLayer::ParallaxLayer(SDL_Texture* texture, int speed)
    : texture(texture), speed(speed) {
    // Obtenemos el ancho de la textura
    SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
}

// Renderiza una capa de parallax
void ParallaxLayer::render(SDL_Renderer* renderer, int cameraX) {
    int renderX = -(cameraX / speed) % width; 

    SDL_Rect srcRect = { 0, 0, width, 960 };
    SDL_Rect destRect = { renderX, 0, width, 960};

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
bool ParallaxBackground::Update(float dt) {
    //updateAndRender();
    Engine::GetInstance().render.get()->DrawTexture(bgTexture1, 0, 0);
    Engine::GetInstance().render.get()->DrawTexture(bgTexture2, 0, 0);

    return true;  // Continue the update cycle
}

void ParallaxBackground::updateAndRender() {

    if (player) {  
        int cameraX = player->position.getX(); 
        
        for (ParallaxLayer& layer : layers) {
            layer.render(renderer, cameraX);
        }
    }
}

void ParallaxBackground::addLayer(SDL_Texture* texture, int speed) {
    layers.emplace_back(texture, speed);
}
